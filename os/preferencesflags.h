#pragma once


////////////////////////////////////////////////////////////////////////////////////////
// preferences flag

namespace vml
{
	struct PreferencesFlags
	{

		// bit flags

		// debug flags

		const static uint32 VERBOSE				 = vml::bits32::BIT0;
		const static uint32 QUIET			     = vml::bits32::BIT1;

		const static uint32 HTMLLOG				 = vml::bits32::BIT30;
		const static uint32 MEMLOG				 = vml::bits32::BIT31;

		// get bit desciptors

		static const std::string ToString(const unsigned int bitvalue)
		{
			switch (bitvalue)
			{

				case VERBOSE:			 return "Verbose";			 break;
				case QUIET:				 return "Quiet";			 break;
				
				case vml::bits32::BIT2:  return "NoDesc02";			 break;
				case vml::bits32::BIT3:  return "NoDesc03";			 break;
				case vml::bits32::BIT4:  return "NoDesc04";			 break;
				case vml::bits32::BIT5:  return "NoDesc05";		 	 break;
				case vml::bits32::BIT6:  return "NoDesc06";          break;
				case vml::bits32::BIT7:  return "NoDesc07";          break;
				case vml::bits32::BIT8:  return "NoDesc08";          break;
				case vml::bits32::BIT9:  return "NoDesc09";          break;
				case vml::bits32::BIT10: return "NoDesc10";          break;
				case vml::bits32::BIT11: return "NoDesc11";          break;
				case vml::bits32::BIT12: return "NoDesc12";          break;
				case vml::bits32::BIT13: return "NoDesc13";          break;
				case vml::bits32::BIT14: return "NoDesc14";			 break;
				case vml::bits32::BIT15: return "NoDesc15";			 break;
				case vml::bits32::BIT16: return "NoDesc16";			 break;
				case vml::bits32::BIT17: return "NoDesc17";			 break;
				case vml::bits32::BIT18: return "NoDesc18";			 break;
				case vml::bits32::BIT19: return "NoDesc19";			 break;
				case vml::bits32::BIT20: return "NoDesc20";			 break;
				case vml::bits32::BIT21: return "NoDesc21";			 break;
				case vml::bits32::BIT22: return "NoDesc22";			 break;
				case vml::bits32::BIT23: return "NoDesc23";			 break;
				case vml::bits32::BIT24: return "NoDesc24";			 break;
				case vml::bits32::BIT25: return "NoDesc25";			 break;
				case vml::bits32::BIT26: return "NoDesc26";			 break;
				case vml::bits32::BIT27: return "NoDesc27";			 break;
				case vml::bits32::BIT28: return "NoDesc28";			 break;
				case vml::bits32::BIT29: return "NoDesc29";			 break;

				case vml::bits32::BIT30: return "HtmlLog";			 break;
				case vml::bits32::BIT31: return "TextLog";			 break;
			}

			return "Preferences flags : Undefined BitValue";
		}

	};
}

