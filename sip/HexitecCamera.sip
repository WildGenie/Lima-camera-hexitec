//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2014
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################

namespace Hexitec
{

class Camera
{

%TypeHeaderCode
#include <HexitecCamera.h>
%End

public:

	enum Status { Ready, Initialising, Exposure, Readout, Fault };
	enum SaveOpt { SaveRaw=1, SaveProcessed=2, SaveHistogram=4 };
	enum ProcessType {RAW,SORT,CSA,CSD,CSA_NF,CSD_NF};

	struct Environment {
		double humidity;
		double ambientTemperature;
		double asicTemperature;
		double adcTemperature;
		double ntcTemperature;
	};

	struct OperatingValues {
		double v3_3;
		double hvMon;
		double hvOut;
		double v1_2;
		double v1_8;
		double v3;
		double v2_5;
		double v3_3ln;
		double v1_65ln;
		double v1_8ana;
		double v3_8ana;
		double peltierCurrent;
		double ntcTemperature;
	};

	Camera(std::string ipAddress, std::string configFilename, int bufferCount=50, int timeout=600);
	~Camera();

	void initialise();
	void startAcq();
	void stopAcq();
	void prepareAcq();
	void reset();

//	void getStatus(Camera::Status& status /Out/);

	// detector info object
	void getPixelSize(double& sizex /Out/, double& sizey /Out/);
	void getDetectorType(std::string& type /Out/);
	void getDetectorModel(std::string& model /Out/);
	void getDetectorImageSize(Size& size /Out/);
	void getDetectorMaxImageSize(Size& size /Out/);

	void getImageType(ImageType& type /Out/);
	void setImageType(ImageType type);

	// synch control object
	void getTrigMode(TrigMode& mode /Out/);
	void setTrigMode(TrigMode mode);
	bool checkTrigMode(TrigMode trig_mode);

	void getExpTime(double& exp_time /Out/);
	void setExpTime(double exp_time);
	void getLatTime(double& lat_time /Out/);
	void setLatTime(double lat_time);
	void getExposureTimeRange(double& min_expo /Out/, double& max_expo /Out/) const;
	void getLatTimeRange(double& min_lat /Out/, double& max_lat /Out/) const;

	void getNbFrames(int& nb_frames /Out/);
	void setNbFrames(int nb_frames);
	void getNbHwAcquiredFrames(int &nb_acq_frames /Out/);

	bool isBinningAvailable();

	// Hexitec specific
	void getEnvironmentalValues(Environment& env /Out/);
	void getOperatingValues(OperatingValues& opval /Out/);
	void getCollectDcTimeout(int& timeout /Out/);
	void setCollectDcTimeout(int timeout);
	void collectOffsetValues();
	void getAsicPitch(int& asicPitch /Out/);
	void setType(ProcessType type);
	void getType(ProcessType& type /Out/);
	void setBinWidth(int binWidth);
	void getBinWidth(int& binWidth /Out/);
	void setSpeclen(int speclen);
	void getSpecLen(int& specLen /Out/);
	void setLowThreshold(int threshold);
	void getLowThreshold(int& threshold /Out/);
	void setHighThreshold(int threshold);
	void getHighThreshold(int& threshold /Out/);
	void getFrameRate(double& rate /Out/);
	void setHvBiasOn();
	void setHvBiasOff();
	void setSaveOpt(int  saveOpt);
	void getSaveOpt(int& saveOpt /Out/);

};

}; // end Hexitec namespace
