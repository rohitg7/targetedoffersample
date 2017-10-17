/***********************************************************/
/* Defines the class object received from the offer service*/
/***********************************************************/

#include "pch.h"
#include "Offer.h"

using namespace Platform;
using namespace TargetOfferSample;
using namespace Windows::Data::Json;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

String^ Offer::trackingIdKey = "trackingId";
String^ Offer::offersKey = "offers";

Offer::Offer(void) : trackingId("")
{
}

Offer::Offer(JsonObject^ jsonObject)
{
	/*
	A sample offer object
	{
		"offers": ["AddOn1", "AddOn2"],
		"trackingId": "0d1b7268-60eb-4e7c-88bb-59edf12da95b"
	}
	*/

	offers = ref new Vector<String^>();
	trackingId = jsonObject->GetNamedString(trackingIdKey, L"");

	// Construct the offers array.
	JsonArray ^jsonOfferArray = jsonObject->GetNamedArray(offersKey);
	for (unsigned int i = 0; i < jsonOfferArray->Size; i++)
	{
		IJsonValue^ jsonValue = jsonOfferArray->GetAt(i);
		if (jsonValue->ValueType == JsonValueType::String)
		{
			offers->Append(jsonValue->GetString());
		}
	}
}

JsonObject^ Offer::ToJsonObject()
{
	JsonObject^ offerObject = ref new JsonObject();
	offerObject->SetNamedValue(trackingIdKey, JsonValue::CreateStringValue(TrackingId));
	return offerObject;
}

String^ Offer::TrackingId::get()
{
	return trackingId;
}

void Offer::TrackingId::set(String^ value)
{
	trackingId = value;
}

IVector<String^>^ Offer::Offers::get()
{
	return offers;
}

void Offer::Offers::set(IVector<String^>^ value)
{
	offers = value;
}
