using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TraceyRzdcxActivation
{
   class Reply
   {
      static public void Success(String s)
      {
         Console.WriteLine("Success: " + s);
      }
      static public void Error(String s)
      {
         Console.WriteLine("Error: " + s);
      }
   }
}
