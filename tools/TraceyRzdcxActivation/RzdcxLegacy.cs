using System;
using TraceyRzdcxActivation.RZDCX;

namespace TraceyRzdcxActivation
{
   class RzdcxLegacy
   {
      ActivationWSSoapClient _wsClient = new ActivationWSSoapClient();
      LegacyActivationRequest _request = new LegacyActivationRequest();

      public RzdcxLegacy()
      {
         Initialize("agatz@traceytech.com", "TRACYTECH-TEST-20210630");
      }

      public RzdcxLegacy(String email, String serialNumber)
      {
         Initialize(email, serialNumber);
      }

      public String Activate(string licenseRequestFileContent)
      {
         _request.LicenseRequestFileContent = licenseRequestFileContent;
         var response = _wsClient.LegacyActivation(_request);

         if (response.Status != ActivationResultCode.Success)
            throw new Exception(response.StatusDescription);

         return response.LicenseFileContent;
      }

      void Initialize(String email, String serialNumber)
      {
         _request.PurchaseInfo = new PurchaseInfo
         {
            CustomerEmail = email,
            SerialNumber = serialNumber
         };
         _request.ProductCode = Product.HRZSDK;
      }
   }
}
