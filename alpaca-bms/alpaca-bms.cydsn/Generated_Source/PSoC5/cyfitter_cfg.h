/*******************************************************************************
* File Name: cyfitter_cfg.h
* 
* PSoC Creator  4.2
*
* Description:
* This file provides basic startup and mux configuration settings
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef CYFITTER_CFG_H
#define CYFITTER_CFG_H

#include "cytypes.h"

extern void cyfitter_cfg(void);

/* Analog Set/Unset methods */
extern void SetAnalogRoutingPumps(uint8 enabled);
extern void MUX_current_Set(uint8 channel);
extern void MUX_current_Unset(uint8 channel);
/* MUX_current__addrTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern uint8 CYXDATA * const CYCODE MUX_current__addrTable[2];
/* MUX_current__maskTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern const uint8 CYCODE MUX_current__maskTable[2];
extern void ADC_current_AMux_Set(uint8 channel);
extern void ADC_current_AMux_Unset(uint8 channel);
/* ADC_current_AMux__addrTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern uint8 CYXDATA * const CYCODE ADC_current_AMux__addrTable[2];
/* ADC_current_AMux__maskTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern const uint8 CYCODE ADC_current_AMux__maskTable[2];


#endif /* CYFITTER_CFG_H */

/*[]*/
