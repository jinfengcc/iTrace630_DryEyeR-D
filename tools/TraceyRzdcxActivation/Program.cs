using System;

namespace TraceyRzdcxActivation
{
   class Program
   {
      static void Main(string[] args)
      {
         try
         {
            if (args.Length != 3)
               throw new Exception("Wrong number of arguments");

            var email = args[0];
            var serNo = args[1];
            var lic = args[2];

            var obj = new RzdcxLegacy(email, serNo);
            Reply.Success(obj.Activate(lic));
         }
         catch (Exception e)
         {
            Reply.Error(e.Message);
         }
      }
   }
}

/*
 * License server:
 * https://licenseroniza.azurewebsites.net/ActivationWS.asmx
 *
 */

/***

To Activate your MODALIZER-SDK license follow this procedure:

1.When you run your software MODALIZER-SDK will create a file called hrzsdk_license_request.txt.

    This file will be in the folder where your software is (your executable) or where RZDCX.DLL is.

2. Open this file with notepad and copy its content to the clipboard. It should start with "3".

3. Follow this link: https://hrzactivation.azurewebsites.net

4.Log in using your serial and email:

    Serial Number: TRACYTECH-TEST-20210630
    E - Mail: agatz@traceytech.com

5. A file named HRZSDK.license will be downloaded.

6. Save this file in the same folder where you found the license request file.

7. Delete the license request file

8. Run your software again - now no license request file is created.The license is activated!

***/