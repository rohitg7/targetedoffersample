#pragma once
namespace TargetOfferSample
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class OfferIds sealed
	{
	public:
		OfferIds(void);
		OfferIds(Windows::Data::Json::JsonObject^ jsonObject);
		Windows::Data::Json::JsonObject^ ToJsonObject();

		// Product Id: product id, for making purchases using windows.applicationmodel.store.currentapp namespace.
		property Platform::String^ ProductId
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		// Product Id: product id, for making purchases using Windows.Services.Store namespace.
		property Platform::String^ StoreId
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

	private:
		static Platform::String^ productIdKey;
		static Platform::String^ storeIdKey;

		Platform::String^ productId;
		Platform::String^ storeId;
	};
}

