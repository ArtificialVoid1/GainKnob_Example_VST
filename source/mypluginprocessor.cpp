//------------------------------------------------------------------------
// Copyright(c) 2024 ArtificialVoid1.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace MyCompanyName {
//------------------------------------------------------------------------
// Gain_KnobProcessor
//------------------------------------------------------------------------
Gain_KnobProcessor::Gain_KnobProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kGain_KnobControllerUID);
}

//------------------------------------------------------------------------
Gain_KnobProcessor::~Gain_KnobProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

	if (data.inputParameterChanges)
	{
		if (data.inputParameterChanges)
		{
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
			for (int32 index = 0; index < numParamsChanged; index++)
			{
				Vst::IParamValueQueue* paramQueue =
					data.inputParameterChanges->getParameterData(index);
				if (paramQueue)
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount();
					switch (paramQueue->getParameterId())
					{
					case params::kGain: //if the param changing is kGain
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
							kResultTrue)
							mGain = (float)value; //<-the important part
						break;
					}
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	Vst::Sample32* InL = data.inputs[0].channelBuffers32[0];
	Vst::Sample32* InR = data.inputs[0].channelBuffers32[1];

	Vst::Sample32* OutL = data.outputs[0].channelBuffers32[0];
	Vst::Sample32* OutR = data.outputs[0].channelBuffers32[1];

	for (int i = 0; i < data.numSamples; i++) { //loop over all samples in block

		OutL[i] = InL[i];
		OutL[i] *= 1 + (9 * mGain);

		OutR[i] = InR[i];
		OutR[i] *= 1 + (9 * mGain);
	}
	

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);

	float savedGain = 0.f;
	if (streamer.readFloat(savedGain) == false)
		return kResultFalse;

	mGain = savedGain;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Gain_KnobProcessor::getState (IBStream* state)
{

	float GainValue = mGain;

	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	streamer.writeFloat(GainValue);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
