//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace TargetOfferSample;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Security::Authentication::Web::Core;
using namespace Windows::Security::Credentials;
using namespace Windows::UI::ApplicationSettings;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Headers;
using namespace concurrency;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	InitializeAccountProvider();
}

// Do basic initialization for the dialog that will pop-up for user login.
void MainPage::InitializeAccountProvider()
{
	AccountsSettingsPane::GetForCurrentView()->AccountCommandsRequested::add(
		ref new TypedEventHandler<AccountsSettingsPane^, AccountsSettingsPaneCommandsRequestedEventArgs^>(
			this,
			&MainPage::OnAccountCommandsRequested)
	);

	concurrency::create_task(WebAuthenticationCoreManager::FindAccountProviderAsync("https://login.microsoft.com", "consumers"))
		.then([this](WebAccountProvider^ msaProvider)
	{
		if (msaProvider)
		{
			m_provider = msaProvider;
		}
	});
}

/*
In case the pane is invked, trigger the actual login command.
*/
void MainPage::OnAccountCommandsRequested(
	AccountsSettingsPane^ sender,
	AccountsSettingsPaneCommandsRequestedEventArgs^ e)
{
	// Create a new WebAccountProviderCommandInvokedHandler for the event fired when a user clicks on a provider in the AccountSettingsPane
	WebAccountProviderCommandInvokedHandler^ handler = ref new WebAccountProviderCommandInvokedHandler(this, &MainPage::WebAccountProviderCommandInvoked);

	// Make a new command based on the MSA provider and the click handler we just created
	WebAccountProviderCommand^ msaProviderCommand = ref new WebAccountProviderCommand(MainPage::m_provider, handler);

	// Append that command to the WebAccountProviderCommands list for the AccountSettingsPane
	e->WebAccountProviderCommands->Append(msaProviderCommand);
}

// The function that will be called from AccountSettingsPane with the WebAccountProvider
// that was clicked by the user
void MainPage::WebAccountProviderCommandInvoked(WebAccountProviderCommand^ command)
{
	// Note the order for scope, i.e. devcenter_implicit.basic should
	auto webTokenRequest = ref new WebTokenRequest(command->WebAccountProvider, "devcenter_implicit.basic,wl.basic");
	// When our task finishes it will return result of the operation, and if successful it will contain a token
	// and WebAccount. We save the WebAccount as the "active" account.
	concurrency::create_task(WebAuthenticationCoreManager::RequestTokenAsync(webTokenRequest)).then([this](WebTokenRequestResult^ webTokenRequestResult)
	{
		if (webTokenRequestResult->ResponseStatus == WebTokenRequestStatus::Success)
		{
			if (webTokenRequestResult->ResponseData->GetAt(0)->Token)
			{
				GetTargetedOffers(webTokenRequestResult->ResponseData->GetAt(0)->Token);
			}
		};
	});
}


void MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Inside OnAccountCommandsRequested we will determine that we need to show accounts
	// and providers eligible for sign in because there is no signed in account
	AccountsSettingsPane::Show();
}

void MainPage::GetTargetedOffers(String^ msaToken)
{
	HttpClient^ httpClient = ref new HttpClient();
	// Put the msa token in the Authorization header as Bearer token.
	httpClient->DefaultRequestHeaders->Authorization = ref new HttpCredentialsHeaderValue("Bearer", msaToken);

	// Request for targeted offer.
	Windows::Foundation::Uri^ localUri = ref new Windows::Foundation::Uri("https://manage.devcenter.microsoft.com/v3.0/my/storeoffers/user");

	create_task(httpClient->GetAsync(localUri)).then(
		[this](HttpResponseMessage^ response)
	{
		// Read the response and deserialize it.
		task<String^> readAsStringTask(response->Content->ReadAsStringAsync());
		return readAsStringTask.then([=](String^ responseBodyAsText) {
			// Convert all instances of <br> to newline.
			std::wstring ws = responseBodyAsText->Data();
			ParseTargetOfferResponse(ref new String(ws.c_str()));
		}, task_continuation_context::use_current());
	});
}

// Returns an array of offer. We are not doing anything with it, but that's where your business logic should come.
IVector<Offer^>^ MainPage::ParseTargetOfferResponse(String^ jsonString)
{
	Vector<Offer^>^ targetedOffers;
	JsonArray^ jsonArray = JsonArray::Parse(jsonString);
	targetedOffers = ref new Vector<Offer^>();
	for (unsigned int i = 0; i < jsonArray->Size; i++)
	{
		IJsonValue^ jsonValue = jsonArray->GetAt(i);
		if (jsonValue->ValueType == JsonValueType::Object)
		{
			targetedOffers->Append(ref new Offer(jsonValue->GetObject()));
		}
	}
	return targetedOffers;
}