//------------------------------------------------------------------------------
// File: oapiimpl.hpp
//
// Desc: OpenCVR API - OpenCVR API.
//
// Copyright (c) 2014-2018 opencvr.com. All rights reserved.
//------------------------------------------------------------------------------
#ifndef __VSC_OAPIS_IMPL_H_
#define __VSC_OAPIS_IMPL_H_

BOOL OAPIConverter::Converter(VSCDeviceData__ &from, oapi::Device &to)
{
	to.nId = from.nId;
	to.nType = from.nType;
	to.nSubType = from.nSubType;
	
	to.Name = from.Name;
	to.Param = from.Param;
	
	to.IP = from.IP;
	to.Port = from.Port;
	to.User = from.User;
	to.Password = from.Password;

	
	to.RtspLocation = from.RtspLocation;
	to.FileLocation = from.FileLocation;
	to.OnvifAddress = from.OnvifAddress;
	to.CameraIndex = from.CameraIndex;

	to.UseProfileToken = from.UseProfileToken;
	to.OnvifProfileToken = from.OnvifProfileToken;

	to.Recording = from.Recording;
	to.GroupId = from.GroupId;
	to.HdfsRecording = from.HdfsRecording;

	to.OnvifProfileToken2 = from.OnvifProfileToken2;

	to.ConnectType = from.ConnectType;
	to.Mining = from.Mining;
	to.HWAccel = from.HWAccel;
	to.IPV6 = from.IPV6;
	
	return TRUE;
}

BOOL OAPIConverter::Converter(VSCDeviceData__ &from, 
						cloudapi::CloudAPIDevice &to)
{
	to.nId = from.nId;
	
	to.Name = from.Name;
	to.Param = from.Param;
	to.SnapshotUrl = from.Name;

	return TRUE;
}

BOOL OAPIConverter::Converter(oapi::Device &from, VSCDeviceData__ &to)
{
	return TRUE;
}

OAPIServer::OAPIServer(XRef<XSocket> pSocket, Factory &pFactory)
:m_pFactory(pFactory), m_pSocket(pSocket), m_nLiveviewId(0), m_cnt(0), 
m_bLogin(FALSE)
{
	UUIDGenerator uuidCreator;
	
	m_seesionId  = uuidCreator.createRandom().toString();
}
OAPIServer::~OAPIServer()
{
	if (m_nLiveviewId > 0)
	{
		m_pFactory.UnRegDataCallback(m_nLiveviewId, (void *)this);
	}
}

BOOL OAPIServer::ProcessStartLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::StartLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	m_pFactory.RegDataCallback(liveview.nId,
		(DeviceDataCallbackFunctionPtr)OAPIServer::DataHandler, 
			(void *)this);
	m_nLiveviewId = liveview.nId;

	delete [] pRecv;

	return TRUE;
}

BOOL OAPIServer::ProcessStopLive(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::StopLiveViewReq liveview;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, liveview, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	m_pFactory.UnRegDataCallback(liveview.nId, (void *)this);
	m_nLiveviewId = 0;

	delete [] pRecv;

	return TRUE;
}

inline BOOL OAPIServer::ProcessLogin(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::LoginReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}
	delete [] pRecv;
	pRecv = NULL;

	/* current only support admin */
	std::string UserAdmin = "admin";

	if (req.User != UserAdmin)
	{
		return FALSE;
	}

	/* Get user data */
	VSCUserData user;
	m_pFactory.GetUserData(user);
	astring realPasswd = user.data.conf.Passwd;

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	oapi::LoginRsp rsp;

	if (md5Output == req.Password)
	{
		rsp.bRet = true;
		m_bLogin = TRUE;
	}else
	{
		rsp.bRetNonce = true;
		rsp.bRet = false;
	}

	rsp.Nonce = m_seesionId;
	
	std::string strJson = autojsoncxx::to_pretty_json_string(rsp);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}

	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_LOGIN_RSP);
	header.length = htonl(nJsonLen + 1);

	
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
	
}

BOOL OAPIServer::ProcessGetDevice(s32 len)
{
	if (len == 0)
	{
		return FALSE;
	}
	char *pRecv = new char[len + 1];
	s32 nRetBody = m_pSocket->Recv((void *)pRecv, len);
	oapi::DeviceListReq req;
	if (nRetBody == len)
	{
		autojsoncxx::ParsingResult result;
		if (!autojsoncxx::from_json_string(pRecv, req, result)) 
		{
			std::cerr << result << '\n';
			delete [] pRecv;
			return FALSE;
		}
		
	}

	oapi::DeviceListRsp dataList;
	dataList.Num = 0;

	DeviceParamMap pDeviceMap;
	DeviceOnlineMap pDeviceOnlineMap;
	m_pFactory.GetDeviceParamMap(pDeviceMap);
	m_pFactory.GetDeviceOnlineMap(pDeviceOnlineMap);

	DeviceParamMap::iterator it = pDeviceMap.begin(); 
	for(; it!=pDeviceMap.end(); ++it)
	{
		oapi::Device data;
		OAPIConverter::Converter(((*it).second).m_Conf.data.conf, data);
		data.Online = pDeviceOnlineMap[(*it).first];
		dataList.list.push_back(data);
		dataList.Num ++;
	}
	//autojsoncxx::to_json_string

	std::string strJson = autojsoncxx::to_pretty_json_string(dataList);
	s32 nJsonLen = strJson.length();
	if (nJsonLen <= 0)
	{
		return FALSE;
	}
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_DEVICE_LIST_RSP);
	header.length = htonl(nJsonLen + 1);

	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)strJson.c_str(), nJsonLen + 1);

	return TRUE;
}

BOOL OAPIServer::Process(OAPIHeader &header)
{
	header.version = ntohl(header.version);
	header.cmd = ntohl(header.cmd);
	header.length = ntohl(header.length);
	if (m_bLogin == FALSE && header.cmd != OAPI_CMD_LOGIN_REQ)
	{
		return FALSE;
	}

	switch(header.cmd)
	{
		case OAPI_CMD_LOGIN_REQ:
			return ProcessLogin(header.length);
			break;
		case OAPI_CMD_KEEPALIVE_REQ:
			break;
		case OAPI_CMD_DEVICE_LIST_REQ:
			return ProcessGetDevice(header.length);
			break;
		case OAPI_CMD_START_LIVE_REQ:
			return ProcessStartLive(header.length);
			break;
		case OAPI_CMD_STOP_LIVE_REQ:
			return ProcessStopLive(header.length);
			break;
		default:
			break;		
	}
	return TRUE;
}

inline void OAPIServer::DataHandler1(VideoFrame& frame)
{
	VideoFrameHeader frameHeader;
	OAPIHeader header;
	header.cmd = htonl(OAPI_CMD_FRAME_PUSH);
	header.length = htonl(sizeof(frameHeader) + frame.dataLen);	

	frameHeader.streamType = htonl(frame.streamType);
	frameHeader.frameType = htonl(frame.frameType);
	frameHeader.secs = htonl(frame.secs);
	frameHeader.msecs = htonl(frame.msecs);
	frameHeader.dataLen = htonl(frame.dataLen);

	//printf("Send a new frame %d\n", m_cnt++);
	m_pSocket->Send((void *)&header, sizeof(header));
	m_pSocket->Send((void *)&frameHeader, sizeof(frameHeader));
	m_pSocket->Send((void *)frame.dataBuf, frame.dataLen);
	
}

inline void OAPIServer::DataHandler(VideoFrame& frame, void * pParam)
{
    OAPIServer *pOapi = static_cast<OAPIServer *> (pParam);
    
    return pOapi->DataHandler1(frame);
}

#endif