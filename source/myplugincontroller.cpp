//------------------------------------------------------------------------
// Copyright(c) 2024 ArtificialVoid1.
//------------------------------------------------------------------------

#include "myplugincontroller.h"
#include "myplugincids.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

using namespace Steinberg;

namespace MyCompanyName {

//------------------------------------------------------------------------
// Gain_KnobController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
	setKnobMode(Vst::KnobModes::kLinearMode);
	parameters.addParameter(STR16("Gain"), STR16("dB"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, params::kGain);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	IBStreamer streamer(state, kLittleEndian);

	float FloatParamSaved = 0.f;
	if (streamer.readFloat(FloatParamSaved) == false) {
		return kResultFalse;
	}
	setParamNormalized(params::kGain, FloatParamSaved);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API Gain_KnobController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "myplugineditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
