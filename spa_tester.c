

/////////////////////////////////////////////
//          SPA TESTER for SPA.C           //
//                                         //
//      Solar Position Algorithm (SPA)     //
//                   for                   //
//        Solar Radiation Application      //
//                                         //
//             August 12, 2004             //
//                                         //
//   Filename: SPA_TESTER.C                //
//                                         //
//   Afshin Michael Andreas                //
//   afshin_andreas@nrel.gov (303)384-6383 //
//                                         //
//   Measurement & Instrumentation Team    //
//   Solar Radiation Research Laboratory   //
//   National Renewable Energy Laboratory  //
//   1617 Cole Blvd, Golden, CO 80401      //
/////////////////////////////////////////////

/////////////////////////////////////////////
// This sample program shows how to use    //
//    the SPA.C code.                      //
/////////////////////////////////////////////


/* Modified by Val Gretchev November 4, 2011 */

/* I had to use the short form floating point
   notation (i.e. - float is IEEE32, double is
   IEEE32). The long form (float is IEEE32,
   double is IEEE64) generates code greater than
   the free compiler limitation of 32768 bytes. */



#include <stdio.h>
#include "spa.h"  //include the SPA header file
#include "Buffers.h"
#include "SCI0.h"
#include "spa_tester.h"


spa_data spa;  //declare the SPA structure



void SPA_Test(void)
{

	int result;
	float min, sec;
	char* BufNum;

	//enter required input values into SPA structure
	spa.year			= 2003;
	spa.month			= 10;
	spa.day				= 17;
	spa.hour			= 12;
	spa.minute			= 30;
	spa.second			= 30;
	spa.timezone		= -7.0;
	spa.delta_t			= 67;
	spa.longitude		= -105.1786;
	spa.latitude		= 39.742476;
	spa.elevation		= 1830.14;
	spa.pressure		= 820;
	spa.temperature		= 11;
	spa.slope			= 30;
	spa.azm_rotation	= -10;
	spa.atmos_refract	= 0.5667;
	spa.function		= SPA_ALL;

	//call the SPA calculate function and pass the SPA structure

	result = spa_calculate(&spa);

	if (result == 0)  //check for SPA errors
	{
		//display the results inside the SPA structure
		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Julian Day:    %.6f\r\n",spa.jd);
		SCI0_PutQueue(BufNum);		

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"L:             %.6e degrees\r\n",spa.l);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"B:             %.6e degrees\r\n",spa.b);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"R:             %.6f AU\r\n",spa.r);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"H:             %.6f degrees\r\n",spa.h);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Delta Psi:     %.6e degrees\r\n",spa.del_psi);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Delta Epsilon: %.6e degrees\r\n",spa.del_epsilon);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Epsilon:       %.6f degrees\r\n",spa.epsilon);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Zenith:        %.6f degrees\r\n",spa.zenith);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Azimuth:       %.6f degrees\r\n",spa.azimuth);
		SCI0_PutQueue(BufNum);

		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Incidence:     %.6f degrees\r\n",spa.incidence);
		SCI0_PutQueue(BufNum);

		min = 60.0*(spa.sunrise - (int)(spa.sunrise));
		sec = 60.0*(min - (int)min);
		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Sunrise:       %02d:%02d:%02d Local Time\r\n", (int)(spa.sunrise), (int)min, (int)sec);
		SCI0_PutQueue(BufNum);

		min = 60.0*(spa.sunset - (int)(spa.sunset));
		sec = 60.0*(min - (int)min);
		BufNum = GetBuffer();
		(void)sprintf(BufNum,"Sunset:        %02d:%02d:%02d Local Time\r\n", (int)(spa.sunset), (int)min, (int)sec);
		SCI0_PutQueue(BufNum);

		SCI0_PutQueue("\r\n");

	}
	else
	{
		BufNum = GetBuffer();
		(void)sprintf(BufNum,"SPA Error Code: %d\r\n", result);
		SCI0_PutQueue(BufNum);
	}
}

////////////////////////////////////////////////////////////////////////////////
// The output of this program should be:	Actual:
//
//Julian Day:    2452930.312847				Julian Day:    2452930.250000
//L:             2.401826e+01 degrees		L:             2.395500e+01 degrees
//B:             -1.011219e-04 degrees		B:             -9.992385e-05 degrees
//R:             0.996542 AU				R:             0.996560 AU
//H:             11.105902 degrees			H:             348.489837 degrees
//Delta Psi:     -3.998404e-03 degrees		Delta Psi:     -3.999539e-03 degrees
//Delta Epsilon: 1.666568e-03 degrees		Delta Epsilon: 1.666405e-03 degrees
//Epsilon:       23.440465 degrees			Epsilon:       23.440462 degrees
//Zenith:        50.111622 degrees			Zenith:        50.167068 degrees
//Azimuth:       194.340241 degrees			Azimuth:       165.144256 degrees
//Incidence:     25.187000 degrees			Incidence:     20.394828 degrees
//Sunrise:       06:12:43 Local Time		Sunrise:       06:12:53 Local Time
//Sunset:        17:20:19 Local Time		Sunset:        17:20:28 Local Time
//
////////////////////////////////////////////////////////////////////////////////

