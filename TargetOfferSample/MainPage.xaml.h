//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Offer.h"

namespace TargetOfferSample
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void InitializeAccountProvider();
		void OnAccountCommandsRequested(Windows::UI::ApplicationSettings::AccountsSettingsPane^ sender, Windows::UI::ApplicationSettings::AccountsSettingsPaneCommandsRequestedEventArgs^ e);
		void GetTargetedOffers(Platform::String^ msaToken);
		Windows::Foundation::Collections::IVector<Offer^>^ ParseTargetOfferResponse(Platform::String^ jsonString);
		void WebAccountProviderCommandInvoked(Windows::UI::ApplicationSettings::WebAccountProviderCommand^ command);

	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		Windows::Security::Credentials::WebAccountProvider^ m_provider;
	};
}
