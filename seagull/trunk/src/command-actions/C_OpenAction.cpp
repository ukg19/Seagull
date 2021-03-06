/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * (c)Copyright 2006 Hewlett-Packard Development Company, LP.
 *
 */

#include "C_OpenAction.hpp"
#include "GeneratorTrace.hpp"
#include "GeneratorError.h"

C_OpenAction::C_OpenAction(T_CmdAction        P_cmdAction,
                                       T_pControllers P_controllers)
  : C_CommandAction (P_cmdAction, P_controllers) {
}


C_OpenAction::~C_OpenAction() {
}


T_exeCode    C_OpenAction::execute(T_pCmd_scenario P_pCmd,
                                   T_pCallContext  P_callCtxt,
                                   C_MessageFrame *P_msg,
                                   C_MessageFrame *P_ref) {
  
  T_exeCode           L_exeCode    = E_EXE_NOERROR ;
  int                 L_id ;
  T_OpenStatus        L_openStatus ;
  struct timezone     L_timeZone                   ;


  
  L_id = m_controllers.m_channel_ctrl->open_local_channel(P_pCmd->m_channel_id,
                                                          m_args,
                                                          P_callCtxt->m_channel_table,
                                                          &L_openStatus);
  
  if (L_id != -1) {
    if (L_openStatus == E_OPEN_DELAYED) {
      L_exeCode = E_EXE_SUSPEND ;
      P_callCtxt->m_suspend_id = L_id ;
      P_callCtxt->m_suspend_msg = P_msg ;
      P_callCtxt->m_channel_id   = P_pCmd->m_channel_id ;
      P_callCtxt->m_channel_received   = P_callCtxt->m_channel_id ;
      gettimeofday(&P_callCtxt->m_current_time, &L_timeZone) ;
      // OPEN DELAYED => do not execute the command
      // just wait for open
    }
  } else {
    GEN_ERROR(E_GEN_FATAL_ERROR, "Open failed");
    L_exeCode = E_EXE_ERROR ;
  }
  return (L_exeCode);
}





