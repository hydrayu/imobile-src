
// $Revision: 1.17 $

/*@{*/
#ifndef PHDBGTRACE_H
#define PHDBGTRACE_H
/*@}*/




#ifdef PHDBG_TRACES 
#define MAX_TRACE_BUFFER	300

#ifndef PHDBG_TRACES_LEVEL_0
#ifndef PHDBG_TRACES_LEVEL_1
#ifndef PHDBG_TRACES_LEVEL_2
#define PHDBG_TRACES_LEVEL_0
#endif
#endif
#endif

	extern char phOsalNfc_DbgTraceBuffer[];

	#ifdef PHDBG_TRACES_LEVEL_0
	
	/*indicates an error that causes a program to abort.*/
	#define PHDBG_FATAL_ERROR(Str)		{\
											snprintf(phOsalNfc_DbgTraceBuffer,MAX_TRACE_BUFFER, \
											"FATAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}

	#define PHDBG_CRITICAL_ERROR(Str)	{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"CRITICAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}
	#define PHDBG_WARNING(Str)
	#define PHDBG_INFO(Str)	
	#endif  /*End of PHDBG_TRACES_LEVEL_0 */

	#ifdef PHDBG_TRACES_LEVEL_1

	/*indicates an error that causes a program to abort.*/
	#define PHDBG_FATAL_ERROR(Str)		{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"FATAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}

	#define PHDBG_CRITICAL_ERROR(Str)	{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
											"CRITICAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}
	/*Normally this macro shall be used indicate system state that might cause problems in future.*/
	#define PHDBG_WARNING(Str)			{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"WARNING :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											phOsalNfc_DbgString (Str);\
											phOsalNfc_DbgString ("\n");\
										}
	#define PHDBG_INFO(Str)	
	#endif /*End of PHDBG_TRACES_LEVEL_1 */

	#ifdef PHDBG_TRACES_LEVEL_2

	/*indicates an error that causes a program to abort.*/
	#define PHDBG_FATAL_ERROR(Str)		{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
											"FATAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}

	#define PHDBG_CRITICAL_ERROR(Str)	{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
											"CRITICAL ERROR in Module :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
															"In Function:%s\n",__FUNCTION__ );	\
											phOsalNfc_DbgString (phOsalNfc_DbgTraceBuffer);\
										}
	/*Normally this macro shall be used indicate system state that might cause problems in future.*/
	#define PHDBG_WARNING(Str)			{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
											"WARNING :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											phOsalNfc_DbgString (Str);\
											phOsalNfc_DbgString ("\n");\
										}
	
	#define PHDBG_INFO(Str)				{\
											snprintf(phOsalNfc_DbgTraceBuffer, MAX_TRACE_BUFFER, \
											"DBG INFO :%s\n",__FILE__);\
											phOsalNfc_DbgString(phOsalNfc_DbgTraceBuffer);\
											phOsalNfc_DbgString (Str);\
											phOsalNfc_DbgString ("\n");\
										}
	


#endif  /*End of PHDBG_TRACES_LEVEL_2 */
#else
#define PHDBG_FATAL_ERROR(Str)
#define PHDBG_CRITICAL_ERROR(Str)	
#define PHDBG_WARNING(Str)
#define PHDBG_INFO(Str)	


#endif /*end of DEBUG trace*/
#endif /* end of PHDBGTRACE_H */
