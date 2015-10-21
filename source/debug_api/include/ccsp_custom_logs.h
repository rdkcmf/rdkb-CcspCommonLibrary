#ifndef  _CCSP_CUSTOML_LOGS_H_ 
#define  _CCSP_CUSTOML_LOGS_H_

#ifdef  __cplusplus
extern "C" {
#endif

//extern void WriteLog(char *msg);
extern void WriteLog(char *msg, ANSC_HANDLE bus_handle, char *Subsytem, char *pParameterName);
extern int GetLogInfo(ANSC_HANDLE bus_handle, char *Subsytem, char *pParameterName);
#endif
