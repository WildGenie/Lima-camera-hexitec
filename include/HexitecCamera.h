//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2016
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
#ifndef HEXITEC_CAMERA_H
#define HEXITEC_CAMERA_H

#include <stdlib.h>
#include <limits>
#include <memory>
#include <future>
#include "lima/HwBufferMgr.h"
#include "lima/HwMaxImageSizeCallback.h"
#include <HexitecApi.h>
#include <HexitecSavingCtrlObj.h>

namespace lima {
namespace Hexitec {

/*******************************************************************
 * \class Camera
 * \brief object controlling the Hexitec camera
 *******************************************************************/
class Camera: public HwMaxImageSizeCallbackGen {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Hexitec");

	friend class Interface;
public:
	Camera(std::string ipAddress, std::string configFilename, int bufferCount=50, int timeout=600);
	~Camera();

	enum Status { Ready, Initialising, Exposure, Readout, Paused, Fault };
	enum SaveOpt { SaveRaw=1, SaveProcessed=2, SaveHistogram=4 };
	enum ProcessType {
		RAW,     ///< Raw data - no correction
		SORT,    ///< Sorted data
		CSA,     ///< charge sharing addition only
		CSD,     ///< charge sharing discrimination only
		CSA_NF,  ///< charge sharing addition with next frame correction
		CSD_NF   ///< charge sharing discrimination with next frame correction
	};

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

	// hw interface
	void initialise();
	void prepareAcq();
	void startAcq();
	void stopAcq();
	void reset();

	// buffer control object
	HwBufferCtrlObj* getBufferCtrlObj();
	// Saving control object
	SavingCtrlObj* getSavingCtrlObj();

	// detector info object
	void getPixelSize(double& sizex, double& sizey);
	void getDetectorType(std::string& type);
	void getDetectorModel(std::string& model);
	void getDetectorImageSize(Size& size);
	void getDetectorMaxImageSize(Size& size);

	void getImageType(ImageType& type);
	void setImageType(ImageType type);

	// synch control object
	void getTrigMode(TrigMode& mode);
	void setTrigMode(TrigMode mode);
	bool checkTrigMode(TrigMode trig_mode);

	void getExpTime(double& exp_time);
	void setExpTime(double exp_time);
	void getLatTime(double& lat_time);
	void setLatTime(double lat_time);
	void getExposureTimeRange(double& min_expo, double& max_expo) const;
	void getLatTimeRange(double& min_expo, double& max_expo) const;

	void getNbFrames(int& nb_frames);
	void setNbFrames(int nb_frames);
	void getNbHwAcquiredFrames(int &nb_acq_frames);

	bool isBinningAvailable();
	Camera::Status getStatus();

	// Hexitec specific
	void getEnvironmentalValues(Environment& env);
	void getOperatingValues(OperatingValues& opval);
	void getCollectDcTimeout(int& timeout);
	void setCollectDcTimeout(int timeout);
	void collectOffsetValues();
	void getAsicPitch(int& asicPitch);
	void setType(ProcessType type);
	void getType(ProcessType& type);
	void setBinWidth(int binWidth);
	void getBinWidth(int& binWidth);
	void setSpeclen(int speclen);
	void getSpecLen(int& specLen);
	void setLowThreshold(int threshold);
	void getLowThreshold(int& threshold);
	void setHighThreshold(int threshold);
	void getHighThreshold(int& threshold);
	void getFrameRate(double& rate);
	void setHvBiasOn();
	void setHvBiasOff();
	void setSaveOpt(int  saveOpt);
	void getSaveOpt(int& saveOpt);

#ifndef SIPCOMPILATION

private:
	class AcqThread;
	class TimerThread;

	// Buffer control object
	SoftBufferCtrlObj m_bufferCtrlObj;
	// Savi control object
	SavingCtrlObj m_savingCtrlObj;

	std::unique_ptr<AcqThread> m_acq_thread;
	std::unique_ptr<TimerThread> m_timer_thread;
	std::unique_ptr<HexitecAPI::HexitecApi> m_hexitec;

	Cond m_cond;
	Cond m_cond_saving;
	volatile bool m_quit;
	volatile bool m_acq_started;
	volatile bool m_thread_running;
	volatile bool m_timer_wait_flag;

	ImageType m_detectorImageType;
	std::string m_detector_type;
	std::string m_detector_model;
	int m_maxImageWidth;
	int m_maxImageHeight;
	int m_x_pixelsize;
	int m_y_pixelsize;
	int m_offset_x;
	int m_offset_y;
	std::string m_ipAddress;
	std::string m_configFilename;
	int m_bufferCount;
	int m_timeout;
	double m_exp_time;
	double m_latency_time;
	double m_frameTime;
	int m_image_number;
	int m_nb_frames;
	Camera::Status m_status;
	TrigMode m_trig_mode;
	int m_collectDcTimeout;
// for processing
	ProcessType m_processType;
	int m_binWidth;
	int m_asicPitch;
	int m_speclen;
	int m_lowThreshold;
	int m_highThreshold;
	int m_saved_frame_nb;
	int m_biasVoltageRefreshInterval;
	int m_biasVoltageRefreshTime;
	int m_biasVoltageSettleTime;
	std::future<void> m_future_result;
	int m_saveOpt;
#endif
};
} // namespace Hexitec
} // namespace lima

#endif // HEXITEC_CAMERA_H
