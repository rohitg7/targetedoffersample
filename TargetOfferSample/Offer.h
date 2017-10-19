#pragma once
#include "OfferIds.h"

namespace TargetOfferSample
{
	// Mark as WebHostHidden since Windows.Data.Json API is not available in JavaScript. 
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class Offer sealed
	{
	public:
		Offer(void);
		Offer(Windows::Data::Json::JsonObject^ jsonObject);
		Windows::Data::Json::JsonObject^ ToJsonObject();

		// Tracking Id: Each request for GetOffer will return a unique tracking id, in case offers are presented
		// It is a GUID currently.
		property Platform::String^ TrackingId
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		// It is an array of Add-On names. You can use these names to make Add-On purchase.
		property Windows::Foundation::Collections::IVector<OfferIds^>^ Offers
		{
			Windows::Foundation::Collections::IVector<OfferIds^>^ get();
			void set(Windows::Foundation::Collections::IVector<OfferIds^>^ value);
		}

	private:
		static Platform::String^ trackingIdKey;
		static Platform::String^ offersKey;

		Platform::String^ trackingId;
		Windows::Foundation::Collections::IVector<OfferIds^>^ offers;
	};
}