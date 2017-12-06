
#ifndef MAGIC_HAND_H
#define MAGIC_HAND_H



// --- Debug ------------------------------

//これはCanvasをシリアルに出力する関数を使うためのものです。本番はコメントアウトすること。
#include "CanvasPrint.h"

// End Debug -------




// --- Track ball ------------------
#include "FingerTrackDriver.h"
#include "FingerTrackSketcher.h"

// End Track ball -------


// --- Canvas ----------
#include "Canvas.h"

// End Canvas -------


// --- Data structure -----------
#include "Queue.h"
#include "CanvasQueue.h"
//#include "LiteralQueue.h"
#include "CharQueue.h"

#include "Stack.h"

// End Data structure --------

// --- Pattern Recognition ---------
#include "Stroke.h"
#include "StrokeDiscriminator.h"

// End Pattern Recognition -----

// --- StrokeAssembler-------
#include "Literal.h"
#include "StrokeAssembler.h"

//End StrokeAssembler----------

// --- Math -------
#include "Fraction.h"
#include "LiteralFraction.h"

#include "Calculator.h"

#include "CalculateController.h"

// End Math ------


// Display
#include "CalculatorDisplay.h"


#endif