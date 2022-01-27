/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccieilluminants.h"

QVector<int> ScCIEIlluminant::wavelengths() const
{
	QVector<int> wls;
	wls.reserve(m_pow.count());

	QMap<int, double>::const_iterator iter;
	for (iter = m_pow.begin(); iter != m_pow.end(); ++iter)
		wls.append(iter.key());

	return wls;
}

ScD50Illuminant::ScD50Illuminant()
{
	m_pow[380] = 24.49;
	m_pow[385] = 27.18;
	m_pow[390] = 29.87;
	m_pow[395] = 39.59;
	m_pow[400] = 49.31;

	m_pow[405] = 52.91;
	m_pow[410] = 56.51;
	m_pow[415] = 58.27;
	m_pow[420] = 60.03;
	m_pow[425] = 58.93;

	m_pow[430] = 57.82;
	m_pow[435] = 66.32;
	m_pow[440] = 74.82;
	m_pow[445] = 81.04;
	m_pow[450] = 87.25;

	m_pow[455] = 88.93;
	m_pow[460] = 90.61;
	m_pow[465] = 90.99;
	m_pow[470] = 91.37;
	m_pow[475] = 93.24;

	m_pow[480] = 95.11;
	m_pow[485] = 93.54;
	m_pow[490] = 91.96;
	m_pow[495] = 93.84;
	m_pow[500] = 95.72;

	m_pow[505] = 96.17;
	m_pow[510] = 96.61;
	m_pow[515] = 96.87;
	m_pow[520] = 97.13;
	m_pow[525] = 99.61;

	m_pow[530] = 102.10;
	m_pow[535] = 101.43;
	m_pow[540] = 100.75;
	m_pow[545] = 101.54;
	m_pow[550] = 102.32;

	m_pow[555] = 101.16;
	m_pow[560] = 100.00;
	m_pow[565] = 98.87;
	m_pow[570] = 97.74;
	m_pow[575] = 98.33;

	m_pow[580] = 98.92;
	m_pow[585] = 96.21;
	m_pow[590] = 93.50;
	m_pow[595] = 95.59;
	m_pow[600] = 97.69;

	m_pow[605] = 98.48;
	m_pow[610] = 99.27;
	m_pow[615] = 99.16;
	m_pow[620] = 99.04;
	m_pow[625] = 97.38;

	m_pow[630] = 95.72;
	m_pow[635] = 97.29;
	m_pow[640] = 98.86;
	m_pow[645] = 97.26;
	m_pow[650] = 95.67;

	m_pow[655] = 96.93;
	m_pow[660] = 98.19;
	m_pow[665] = 100.60;
	m_pow[670] = 103.00;
	m_pow[675] = 101.07;

	m_pow[680] = 99.13;
	m_pow[685] = 93.26;
	m_pow[690] = 87.38;
	m_pow[695] = 89.49;
	m_pow[700] = 91.60;

	m_pow[705] = 92.25;
	m_pow[710] = 92.89;
	m_pow[715] = 84.87;
	m_pow[720] = 76.85;
	m_pow[725] = 81.68;

	m_pow[730] = 86.51;
	m_pow[735] = 89.55;
	m_pow[740] = 92.58;
	m_pow[745] = 85.40;
	m_pow[750] = 78.23;

	m_pow[755] = 67.96;
	m_pow[760] = 57.69;
	m_pow[765] = 70.31;
	m_pow[770] = 82.92;
	m_pow[775] = 80.60;

	m_pow[780] = 78.27;
}

ScD65Illuminant::ScD65Illuminant()
{
	m_pow[380] = 49.98;
	m_pow[385] = 52.31;
	m_pow[390] = 54.65;
	m_pow[395] = 68.70;
	m_pow[400] = 82.75;

	m_pow[405] = 87.12;
	m_pow[410] = 91.49;
	m_pow[415] = 92.46;
	m_pow[420] = 93.43;
	m_pow[425] = 90.06;

	m_pow[430] = 86.68;
	m_pow[435] = 95.77;
	m_pow[440] = 104.86;
	m_pow[445] = 110.94;
	m_pow[450] = 117.01;

	m_pow[455] = 117.41;
	m_pow[460] = 117.81;
	m_pow[465] = 116.34;
	m_pow[470] = 114.86;
	m_pow[475] = 115.39;

	m_pow[480] = 115.92;
	m_pow[485] = 112.37;
	m_pow[490] = 108.81;
	m_pow[495] = 109.08;
	m_pow[500] = 109.35;

	m_pow[505] = 108.58;
	m_pow[510] = 107.80;
	m_pow[515] = 106.30;
	m_pow[520] = 104.79;
	m_pow[525] = 106.24;

	m_pow[530] = 107.69;
	m_pow[535] = 106.05;
	m_pow[540] = 104.41;
	m_pow[545] = 104.23;
	m_pow[550] = 104.05;

	m_pow[555] = 102.02;
	m_pow[560] = 100.00;
	m_pow[565] = 98.17;
	m_pow[570] = 96.33;
	m_pow[575] = 96.06;

	m_pow[580] = 95.79;
	m_pow[585] = 92.24;
	m_pow[590] = 88.69;
	m_pow[595] = 89.35;
	m_pow[600] = 90.01;

	m_pow[605] = 89.80;
	m_pow[610] = 89.60;
	m_pow[615] = 88.65;
	m_pow[620] = 87.70;
	m_pow[625] = 85.49;

	m_pow[630] = 83.29;
	m_pow[635] = 83.49;
	m_pow[640] = 83.70;
	m_pow[645] = 81.86;
	m_pow[650] = 80.03;

	m_pow[655] = 80.12;
	m_pow[660] = 80.21;
	m_pow[665] = 81.25;
	m_pow[670] = 82.28;
	m_pow[675] = 80.28;

	m_pow[680] = 78.28;
	m_pow[685] = 74.00;
	m_pow[690] = 69.72;
	m_pow[695] = 70.67;
	m_pow[700] = 71.61;

	m_pow[705] = 72.98;
	m_pow[710] = 74.35;
	m_pow[715] = 67.98;
	m_pow[720] = 61.60;
	m_pow[725] = 65.74;

	m_pow[730] = 69.89;
	m_pow[735] = 72.49;
	m_pow[740] = 75.09;
	m_pow[745] = 69.34;
	m_pow[750] = 63.59;

	m_pow[755] = 55.01;
	m_pow[760] = 46.42;
	m_pow[765] = 56.61;
	m_pow[770] = 66.81;
	m_pow[775] = 65.09;

	m_pow[780] = 63.38;
}