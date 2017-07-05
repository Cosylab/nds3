/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDS3_H
#define NDS3_H

/**
 * @file nds.h
 *
 * Projects that use the NDS3 library can include this file which takes care
 * of including all the other necessary files.
 *
 */

#include "nds3/definitions.h"
#include "nds3/exceptions.h"
#include "nds3/base.h"
#include "nds3/iniFileParser.h"
#include "nds3/node.h"
#include "nds3/port.h"
#include "nds3/pvBaseIn.h"
#include "nds3/pvBaseOut.h"
#include "nds3/pvAction.h"
#include "nds3/pvDelegateIn.h"
#include "nds3/pvDelegateOut.h"
#include "nds3/pvVariableIn.h"
#include "nds3/pvVariableOut.h"
#include "nds3/dataAcquisition.h"
#include "nds3/factory.h"
#include "nds3/stateMachine.h"
#include "nds3/thread.h"
#include "nds3/registerDevice.h"


#endif // NDS3_H
