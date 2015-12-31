#pragma once

// --- hrfm::display

#include "DisplayNode.h"
#include "TextureNode.h"
#include "MovieTexture.h"
#include "Stage.h"
#include "BatchNode.h"

// --- hrfm::events

#include "Event.h"
#include "EventDispatcher.h"
#include "SiEventDispatcher.h"

#include "KORGMIDIInterfaceEvent.h"
#include "OscEvent.h"
#include "TimeUtilEvent.h"
#include "TriggerEvent.h"
#include "VolumeDetectorEvent.h"

// --- hrfm::gl

#include "hrfm.gl.h"

#include "FilterHeaders.h"

#include "TransitionShaderBase.h"
#include "MatrixTransition.h"

// --- hrfm::geom;

#include "hrfm.geom.h"

// --- hrfm::io

#include "SiAudioInput.h"
#include "SiCaptureInput.h"
#include "SiKORGMIDIInterface.h"
#include "SiOscInput.h"

#include "DataLoader.h"
#include "XmlLoader.h"

#include "SiVolumeDetector.h"

// --- hrfm::physics

#include "DeclineWave.h"
#include "WaveNode.h"

// --- hrfm::structure

#include "ValueMatrix.h"

// --- hrfm::utils

#include "DisplayUtil.h"

// --- hrfm::app

#include "AppBase.h"
#include "SignageAppBase.h"

// --- hrfm::cv

#include "Delaunay.h"