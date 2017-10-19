/***********************************************************/
/* Defines the class object received from the offer service*/
/***********************************************************/

#include "pch.h"
#include "OfferIds.h"

using namespace Platform;
using namespace TargetOfferSample;
using namespace Windows::Data::Json;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

String^ OfferIds::productIdKey = "productId";
String^ OfferIds::storeIdKey = "storeId";

OfferIds::OfferIds(void) : productId(""), storeId("")
{
}


OfferIds::OfferIds(JsonObject^ jsonObject)
{
	/*
	A sample ProductInfo object
	{"productId": "AddOn1","storeId": "9PK8VD14FRCZ"}
	*/

	productId = jsonObject->GetNamedString(productIdKey, L"");
	storeId = jsonObject->GetNamedString(storeIdKey, L"");
}

JsonObject^ OfferIds::ToJsonObject()
{
	JsonObject^ productInfoObject = ref new JsonObject();
	productInfoObject->SetNamedValue(productIdKey, JsonValue::CreateStringValue(ProductId));
	productInfoObject->SetNamedValue(storeIdKey, JsonValue::CreateStringValue(StoreId));
	return productInfoObject;
}

String^ OfferIds::ProductId::get()
{
	return productId;
}

void OfferIds::ProductId::set(String^ value)
{
	productId = value;
}

String^ OfferIds::StoreId::get()
{
	return storeId;
}

void OfferIds::StoreId::set(String^ value)
{
	storeId = value;
}
