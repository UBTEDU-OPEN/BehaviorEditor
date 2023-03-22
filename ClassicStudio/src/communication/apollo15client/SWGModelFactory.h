/**
 * Yanshee RESTful API
 *  ## 概述 Yanshee RESTful APIs是由使用[swagger-codegen](https://github.com/swagger-api/swagger-codegen)基于 [OpenAPI-Spec](https://github.com/swagger-api/swagger-core/wiki)的工程。 所有的API由Flask的[Connexion](https://github.com/zalando/connexion)来解释。 Yanshee-ROS中的apollo提供所有的RESTful APIs服务。 同时Yanshee RESTful APIs提供英文和中文两个版本： - [英文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_en/1.0.0) - [中文版本](https://app.swaggerhub.com/apis-docs/UBTEDU/apollo_cn/1.0.0)  ## 依赖 Python 2.7 or 3.4+  
 *
 * OpenAPI spec version: 1.0.0
 * Contact: swenggroup@ubtrobot.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

#ifndef ModelFactory_H_
#define ModelFactory_H_

#include "SWGObject.h"

#include "SWGCommonResponse.h"
#include "SWGDevicesBattery.h"
#include "SWGDevicesBatteryResponse.h"
#include "SWGDevicesFallManagement.h"
#include "SWGDevicesFallManagementResponse.h"
#include "SWGDevicesLED.h"
#include "SWGDevicesLEDResponse.h"
#include "SWGDevicesLanguage.h"
#include "SWGDevicesLanguageResponse.h"
#include "SWGDevicesVersions.h"
#include "SWGDevicesVersionsResponse.h"
#include "SWGDevicesVolume.h"
#include "SWGDevicesVolumeResponse.h"
#include "SWGMediaMusicList.h"
#include "SWGMediaMusicListResponse.h"
#include "SWGMediaMusicOperation.h"
#include "SWGMediaMusicStatus.h"
#include "SWGMediaMusicStatusResponse.h"
#include "SWGMotionsGaitOperation.h"
#include "SWGMotionsInfo.h"
#include "SWGMotionsList.h"
#include "SWGMotionsListResponse.h"
#include "SWGMotionsOperation.h"
#include "SWGMotionsParameter.h"
#include "SWGMotionsStatus.h"
#include "SWGMotionsStatusResponse.h"
#include "SWGName.h"
#include "SWGRuntimeResponse.h"
#include "SWGSensorsEnvironmentInfo.h"
#include "SWGSensorsEnvironmentValue.h"
#include "SWGSensorsEnvironmentValueResponse.h"
#include "SWGSensorsGyroInfo.h"
#include "SWGSensorsGyroValue.h"
#include "SWGSensorsGyroValueResponse.h"
#include "SWGSensorsInfo.h"
#include "SWGSensorsInfraredInfo.h"
#include "SWGSensorsInfraredValue.h"
#include "SWGSensorsInfraredValueResponse.h"
#include "SWGSensorsList.h"
#include "SWGSensorsListResponse.h"
#include "SWGSensorsOperationRequest.h"
#include "SWGSensorsParameter.h"
#include "SWGSensorsPressureInfo.h"
#include "SWGSensorsPressureValue.h"
#include "SWGSensorsPressureValueResponse.h"
#include "SWGSensorsTouchInfo.h"
#include "SWGSensorsTouchValue.h"
#include "SWGSensorsTouchValueResponse.h"
#include "SWGSensorsUltrasonicInfo.h"
#include "SWGSensorsUltrasonicValue.h"
#include "SWGSensorsUltrasonicValueResponse.h"
#include "SWGServosAngles.h"
#include "SWGServosAnglesRequest.h"
#include "SWGServosAnglesResponse.h"
#include "SWGServosHands.h"
#include "SWGServosHandsRequest.h"
#include "SWGServosHandsResponse.h"
#include "SWGServosList.h"
#include "SWGServosMode.h"
#include "SWGServosModeRequest.h"
#include "SWGServosModeResponse.h"
#include "SWGServosResult.h"
#include "SWGServosResultResponse.h"
#include "SWGSubscriptionsAsrVoice.h"
#include "SWGSubscriptionsAsrVoiceDelete.h"
#include "SWGSubscriptionsIatVoice.h"
#include "SWGSubscriptionsIatVoiceDelete.h"
#include "SWGSubscriptionsMotions.h"
#include "SWGSubscriptionsMotionsDelete.h"
#include "SWGSubscriptionsMotionsGait.h"
#include "SWGSubscriptionsMotionsGaitDelete.h"
#include "SWGSubscriptionsSensors.h"
#include "SWGSubscriptionsSensorsDelete.h"
#include "SWGSubscriptionsTtsVoice.h"
#include "SWGSubscriptionsTtsVoiceDelete.h"
#include "SWGSubscriptionsVisions.h"
#include "SWGSubscriptionsVisionsDelete.h"
#include "SWGTotalTime.h"
#include "SWGUserTime.h"
#include "SWGVisionsAnalysis.h"
#include "SWGVisionsDeleteTags.h"
#include "SWGVisionsGetResponse.h"
#include "SWGVisionsPhoto.h"
#include "SWGVisionsPhotoListResponse.h"
#include "SWGVisionsPhotoResponse.h"
#include "SWGVisionsPut.h"
#include "SWGVisionsPutResponse.h"
#include "SWGVisionsPutTags.h"
#include "SWGVisionsQuantity.h"
#include "SWGVisionsResults.h"
#include "SWGVisionsStream.h"
#include "SWGVisionsTagsResponse.h"
#include "SWGVisionsTask.h"
#include "SWGVoiceAsrOption.h"
#include "SWGVoiceDeleteOfflineSyntax.h"
#include "SWGVoiceGetOfflineSyntaxGrammarsResponse.h"
#include "SWGVoiceGetOfflineSyntaxResponse.h"
#include "SWGVoiceGetResponse.h"
#include "SWGVoiceIatRequest.h"
#include "SWGVoiceOfflineSlot.h"
#include "SWGVoiceOfflineSyntaxRule.h"
#include "SWGVoicePostOfflineSyntax.h"
#include "SWGVoicePutOfflineSyntax.h"
#include "SWGVoiceResponse.h"
#include "SWGVoiceTTSStr.h"

namespace Swagger {

  inline void* create(QString type) {
    if(QString("SWGCommonResponse").compare(type) == 0) {
      return new SWGCommonResponse();
    }
    if(QString("SWGDevicesBattery").compare(type) == 0) {
      return new SWGDevicesBattery();
    }
    if(QString("SWGDevicesBatteryResponse").compare(type) == 0) {
      return new SWGDevicesBatteryResponse();
    }
    if(QString("SWGDevicesFallManagement").compare(type) == 0) {
      return new SWGDevicesFallManagement();
    }
    if(QString("SWGDevicesFallManagementResponse").compare(type) == 0) {
      return new SWGDevicesFallManagementResponse();
    }
    if(QString("SWGDevicesLED").compare(type) == 0) {
      return new SWGDevicesLED();
    }
    if(QString("SWGDevicesLEDResponse").compare(type) == 0) {
      return new SWGDevicesLEDResponse();
    }
    if(QString("SWGDevicesLanguage").compare(type) == 0) {
      return new SWGDevicesLanguage();
    }
    if(QString("SWGDevicesLanguageResponse").compare(type) == 0) {
      return new SWGDevicesLanguageResponse();
    }
    if(QString("SWGDevicesVersions").compare(type) == 0) {
      return new SWGDevicesVersions();
    }
    if(QString("SWGDevicesVersionsResponse").compare(type) == 0) {
      return new SWGDevicesVersionsResponse();
    }
    if(QString("SWGDevicesVolume").compare(type) == 0) {
      return new SWGDevicesVolume();
    }
    if(QString("SWGDevicesVolumeResponse").compare(type) == 0) {
      return new SWGDevicesVolumeResponse();
    }
    if(QString("SWGMediaMusicList").compare(type) == 0) {
      return new SWGMediaMusicList();
    }
    if(QString("SWGMediaMusicListResponse").compare(type) == 0) {
      return new SWGMediaMusicListResponse();
    }
    if(QString("SWGMediaMusicOperation").compare(type) == 0) {
      return new SWGMediaMusicOperation();
    }
    if(QString("SWGMediaMusicStatus").compare(type) == 0) {
      return new SWGMediaMusicStatus();
    }
    if(QString("SWGMediaMusicStatusResponse").compare(type) == 0) {
      return new SWGMediaMusicStatusResponse();
    }
    if(QString("SWGMotionsGaitOperation").compare(type) == 0) {
      return new SWGMotionsGaitOperation();
    }
    if(QString("SWGMotionsInfo").compare(type) == 0) {
      return new SWGMotionsInfo();
    }
    if(QString("SWGMotionsList").compare(type) == 0) {
      return new SWGMotionsList();
    }
    if(QString("SWGMotionsListResponse").compare(type) == 0) {
      return new SWGMotionsListResponse();
    }
    if(QString("SWGMotionsOperation").compare(type) == 0) {
      return new SWGMotionsOperation();
    }
    if(QString("SWGMotionsParameter").compare(type) == 0) {
      return new SWGMotionsParameter();
    }
    if(QString("SWGMotionsStatus").compare(type) == 0) {
      return new SWGMotionsStatus();
    }
    if(QString("SWGMotionsStatusResponse").compare(type) == 0) {
      return new SWGMotionsStatusResponse();
    }
    if(QString("SWGName").compare(type) == 0) {
      return new SWGName();
    }
    if(QString("SWGRuntimeResponse").compare(type) == 0) {
      return new SWGRuntimeResponse();
    }
    if(QString("SWGSensorsEnvironmentInfo").compare(type) == 0) {
      return new SWGSensorsEnvironmentInfo();
    }
    if(QString("SWGSensorsEnvironmentValue").compare(type) == 0) {
      return new SWGSensorsEnvironmentValue();
    }
    if(QString("SWGSensorsEnvironmentValueResponse").compare(type) == 0) {
      return new SWGSensorsEnvironmentValueResponse();
    }
    if(QString("SWGSensorsGyroInfo").compare(type) == 0) {
      return new SWGSensorsGyroInfo();
    }
    if(QString("SWGSensorsGyroValue").compare(type) == 0) {
      return new SWGSensorsGyroValue();
    }
    if(QString("SWGSensorsGyroValueResponse").compare(type) == 0) {
      return new SWGSensorsGyroValueResponse();
    }
    if(QString("SWGSensorsInfo").compare(type) == 0) {
      return new SWGSensorsInfo();
    }
    if(QString("SWGSensorsInfraredInfo").compare(type) == 0) {
      return new SWGSensorsInfraredInfo();
    }
    if(QString("SWGSensorsInfraredValue").compare(type) == 0) {
      return new SWGSensorsInfraredValue();
    }
    if(QString("SWGSensorsInfraredValueResponse").compare(type) == 0) {
      return new SWGSensorsInfraredValueResponse();
    }
    if(QString("SWGSensorsList").compare(type) == 0) {
      return new SWGSensorsList();
    }
    if(QString("SWGSensorsListResponse").compare(type) == 0) {
      return new SWGSensorsListResponse();
    }
    if(QString("SWGSensorsOperationRequest").compare(type) == 0) {
      return new SWGSensorsOperationRequest();
    }
    if(QString("SWGSensorsParameter").compare(type) == 0) {
      return new SWGSensorsParameter();
    }
    if(QString("SWGSensorsPressureInfo").compare(type) == 0) {
      return new SWGSensorsPressureInfo();
    }
    if(QString("SWGSensorsPressureValue").compare(type) == 0) {
      return new SWGSensorsPressureValue();
    }
    if(QString("SWGSensorsPressureValueResponse").compare(type) == 0) {
      return new SWGSensorsPressureValueResponse();
    }
    if(QString("SWGSensorsTouchInfo").compare(type) == 0) {
      return new SWGSensorsTouchInfo();
    }
    if(QString("SWGSensorsTouchValue").compare(type) == 0) {
      return new SWGSensorsTouchValue();
    }
    if(QString("SWGSensorsTouchValueResponse").compare(type) == 0) {
      return new SWGSensorsTouchValueResponse();
    }
    if(QString("SWGSensorsUltrasonicInfo").compare(type) == 0) {
      return new SWGSensorsUltrasonicInfo();
    }
    if(QString("SWGSensorsUltrasonicValue").compare(type) == 0) {
      return new SWGSensorsUltrasonicValue();
    }
    if(QString("SWGSensorsUltrasonicValueResponse").compare(type) == 0) {
      return new SWGSensorsUltrasonicValueResponse();
    }
    if(QString("SWGServosAngles").compare(type) == 0) {
      return new SWGServosAngles();
    }
    if(QString("SWGServosAnglesRequest").compare(type) == 0) {
      return new SWGServosAnglesRequest();
    }
    if(QString("SWGServosAnglesResponse").compare(type) == 0) {
      return new SWGServosAnglesResponse();
    }
    if(QString("SWGServosHands").compare(type) == 0) {
      return new SWGServosHands();
    }
    if(QString("SWGServosHandsRequest").compare(type) == 0) {
      return new SWGServosHandsRequest();
    }
    if(QString("SWGServosHandsResponse").compare(type) == 0) {
      return new SWGServosHandsResponse();
    }
    if(QString("SWGServosList").compare(type) == 0) {
      return new SWGServosList();
    }
    if(QString("SWGServosMode").compare(type) == 0) {
      return new SWGServosMode();
    }
    if(QString("SWGServosModeRequest").compare(type) == 0) {
      return new SWGServosModeRequest();
    }
    if(QString("SWGServosModeResponse").compare(type) == 0) {
      return new SWGServosModeResponse();
    }
    if(QString("SWGServosResult").compare(type) == 0) {
      return new SWGServosResult();
    }
    if(QString("SWGServosResultResponse").compare(type) == 0) {
      return new SWGServosResultResponse();
    }
    if(QString("SWGSubscriptionsAsrVoice").compare(type) == 0) {
      return new SWGSubscriptionsAsrVoice();
    }
    if(QString("SWGSubscriptionsAsrVoiceDelete").compare(type) == 0) {
      return new SWGSubscriptionsAsrVoiceDelete();
    }
    if(QString("SWGSubscriptionsIatVoice").compare(type) == 0) {
      return new SWGSubscriptionsIatVoice();
    }
    if(QString("SWGSubscriptionsIatVoiceDelete").compare(type) == 0) {
      return new SWGSubscriptionsIatVoiceDelete();
    }
    if(QString("SWGSubscriptionsMotions").compare(type) == 0) {
      return new SWGSubscriptionsMotions();
    }
    if(QString("SWGSubscriptionsMotionsDelete").compare(type) == 0) {
      return new SWGSubscriptionsMotionsDelete();
    }
    if(QString("SWGSubscriptionsMotionsGait").compare(type) == 0) {
      return new SWGSubscriptionsMotionsGait();
    }
    if(QString("SWGSubscriptionsMotionsGaitDelete").compare(type) == 0) {
      return new SWGSubscriptionsMotionsGaitDelete();
    }
    if(QString("SWGSubscriptionsSensors").compare(type) == 0) {
      return new SWGSubscriptionsSensors();
    }
    if(QString("SWGSubscriptionsSensorsDelete").compare(type) == 0) {
      return new SWGSubscriptionsSensorsDelete();
    }
    if(QString("SWGSubscriptionsTtsVoice").compare(type) == 0) {
      return new SWGSubscriptionsTtsVoice();
    }
    if(QString("SWGSubscriptionsTtsVoiceDelete").compare(type) == 0) {
      return new SWGSubscriptionsTtsVoiceDelete();
    }
    if(QString("SWGSubscriptionsVisions").compare(type) == 0) {
      return new SWGSubscriptionsVisions();
    }
    if(QString("SWGSubscriptionsVisionsDelete").compare(type) == 0) {
      return new SWGSubscriptionsVisionsDelete();
    }
    if(QString("SWGTotalTime").compare(type) == 0) {
      return new SWGTotalTime();
    }
    if(QString("SWGUserTime").compare(type) == 0) {
      return new SWGUserTime();
    }
    if(QString("SWGVisionsAnalysis").compare(type) == 0) {
      return new SWGVisionsAnalysis();
    }
    if(QString("SWGVisionsDeleteTags").compare(type) == 0) {
      return new SWGVisionsDeleteTags();
    }
    if(QString("SWGVisionsGetResponse").compare(type) == 0) {
      return new SWGVisionsGetResponse();
    }
    if(QString("SWGVisionsPhoto").compare(type) == 0) {
      return new SWGVisionsPhoto();
    }
    if(QString("SWGVisionsPhotoListResponse").compare(type) == 0) {
      return new SWGVisionsPhotoListResponse();
    }
    if(QString("SWGVisionsPhotoResponse").compare(type) == 0) {
      return new SWGVisionsPhotoResponse();
    }
    if(QString("SWGVisionsPut").compare(type) == 0) {
      return new SWGVisionsPut();
    }
    if(QString("SWGVisionsPutResponse").compare(type) == 0) {
      return new SWGVisionsPutResponse();
    }
    if(QString("SWGVisionsPutTags").compare(type) == 0) {
      return new SWGVisionsPutTags();
    }
    if(QString("SWGVisionsQuantity").compare(type) == 0) {
      return new SWGVisionsQuantity();
    }
    if(QString("SWGVisionsResults").compare(type) == 0) {
      return new SWGVisionsResults();
    }
    if(QString("SWGVisionsStream").compare(type) == 0) {
      return new SWGVisionsStream();
    }
    if(QString("SWGVisionsTagsResponse").compare(type) == 0) {
      return new SWGVisionsTagsResponse();
    }
    if(QString("SWGVisionsTask").compare(type) == 0) {
      return new SWGVisionsTask();
    }
    if(QString("SWGVoiceAsrOption").compare(type) == 0) {
      return new SWGVoiceAsrOption();
    }
    if(QString("SWGVoiceDeleteOfflineSyntax").compare(type) == 0) {
      return new SWGVoiceDeleteOfflineSyntax();
    }
    if(QString("SWGVoiceGetOfflineSyntaxGrammarsResponse").compare(type) == 0) {
      return new SWGVoiceGetOfflineSyntaxGrammarsResponse();
    }
    if(QString("SWGVoiceGetOfflineSyntaxResponse").compare(type) == 0) {
      return new SWGVoiceGetOfflineSyntaxResponse();
    }
    if(QString("SWGVoiceGetResponse").compare(type) == 0) {
      return new SWGVoiceGetResponse();
    }
    if(QString("SWGVoiceIatRequest").compare(type) == 0) {
      return new SWGVoiceIatRequest();
    }
    if(QString("SWGVoiceOfflineSlot").compare(type) == 0) {
      return new SWGVoiceOfflineSlot();
    }
    if(QString("SWGVoiceOfflineSyntaxRule").compare(type) == 0) {
      return new SWGVoiceOfflineSyntaxRule();
    }
    if(QString("SWGVoicePostOfflineSyntax").compare(type) == 0) {
      return new SWGVoicePostOfflineSyntax();
    }
    if(QString("SWGVoicePutOfflineSyntax").compare(type) == 0) {
      return new SWGVoicePutOfflineSyntax();
    }
    if(QString("SWGVoiceResponse").compare(type) == 0) {
      return new SWGVoiceResponse();
    }
    if(QString("SWGVoiceTTSStr").compare(type) == 0) {
      return new SWGVoiceTTSStr();
    }
    
    return nullptr;
  }

  inline void* create(QString json, QString type) {
    if(type.startsWith("QString")) {
      return new QString();
    }    
    auto val = static_cast<SWGObject*>(create(type));
    if(val != nullptr) {
      return val->fromJson(json);
    }
    return nullptr;
  }

}

#endif /* ModelFactory_H_ */
