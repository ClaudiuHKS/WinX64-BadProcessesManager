
#include < nlohmann/json.hpp >

#include < iostream >
#include < fstream >

#include < winsock2.h >
#include < psapi.h >
#include < wbemidl.h >
#include < windows.h >
#include < tlhelp32.h >

class queryProcData
{
public:

    ::IWbemLocator* pWbemLocator{ };
    ::IWbemServices* pWbemServices{ };
    ::IWbemClassObject* pWbemClassObject{ };
    ::IEnumWbemClassObject* pEnumWbemClassObject{ };

    wchar_t* pQueryLangSysString{ };
    wchar_t* pQuerySysString{ };
    wchar_t* pNetworkResourceSysString{ };

    ::tagVARIANT ProcessIndex{ };
    ::tagVARIANT ProcessCommandLine{ };
    ::tagVARIANT ProcessExePath{ };
};

bool readUsePipe() noexcept;
unsigned long readProcRunInterval() noexcept;

::std::string inverseSlashes(::std::string _) noexcept
{
    if (_.empty())
    {
        return _;
    }

    for (auto& _ : _)
    {
        if (_ == '\\')
        {
            _ = '/';
        }

        else if (_ == '/')
        {
            _ = '\\';
        }
    }

    return _;
}

::std::wstring inverseSlashes(::std::wstring _) noexcept
{
    if (_.empty())
    {
        return _;
    }

    for (auto& _ : _)
    {
        if (_ == L'\\')
        {
            _ = L'/';
        }

        else if (_ == L'/')
        {
            _ = L'\\';
        }
    }

    return _;
}

::std::wstring toUnicode(::std::string _) noexcept
{
    static wchar_t wszStr[4096I16]{ };
    {
        ::memset(wszStr, { }, sizeof wszStr);
        {
            ::MultiByteToWideChar(((unsigned int)(65001UI16)), { }, _.c_str(), ((int)(-1I8)), wszStr, ((((int)(sizeof wszStr)) / ((int)(sizeof(decltype (*wszStr))))) - ((int)(1I8))));
            {
                return wszStr;
            }
        }
    }
}

::std::string toMultiByte(::std::wstring _) noexcept
{
    static char szStr[4096I16]{ };
    {
        ::memset(szStr, { }, sizeof szStr);
        {
            ::WideCharToMultiByte(((unsigned int)(65001UI16)), { }, _.c_str(), ((int)(-1I8)), szStr, ((((int)(sizeof szStr)) / ((int)(sizeof(decltype (*szStr))))) - ((int)(1I8))), nullptr, nullptr);
            {
                return szStr;
            }
        }
    }
}

void queryProcFreeData(::queryProcData& _) noexcept
{
    if (_.pWbemLocator)
    {
        _.pWbemLocator->Release();
        {
            _.pWbemLocator = { };
        }
    }

    if (_.pWbemServices)
    {
        _.pWbemServices->Release();
        {
            _.pWbemServices = { };
        }
    }

    if (_.pWbemClassObject)
    {
        _.pWbemClassObject->Release();
        {
            _.pWbemClassObject = { };
        }
    }

    if (_.pEnumWbemClassObject)
    {
        _.pEnumWbemClassObject->Release();
        {
            _.pEnumWbemClassObject = { };
        }
    }

    if (_.pQueryLangSysString)
    {
        ::SysFreeString(_.pQueryLangSysString);
        {
            _.pQueryLangSysString = { };
        }
    }

    if (_.pQuerySysString)
    {
        ::SysFreeString(_.pQuerySysString);
        {
            _.pQuerySysString = { };
        }
    }

    if (_.pNetworkResourceSysString)
    {
        ::SysFreeString(_.pNetworkResourceSysString);
        {
            _.pNetworkResourceSysString = { };
        }
    }

    ::VariantClear(&_.ProcessIndex);
    {
        _.ProcessIndex = { };
    }

    ::VariantClear(&_.ProcessCommandLine);
    {
        _.ProcessCommandLine = { };
    }

    ::VariantClear(&_.ProcessExePath);
    {
        _.ProcessExePath = { };
    }

    _ = { };
}

bool queryProcesses(::std::vector < unsigned long >& vecProcesses, ::std::vector < ::std::string >& vecCommandLines, ::std::vector < ::std::string >& vecExePaths) noexcept
{
    static ::queryProcData Var{ };

    static unsigned long _{ };

    vecProcesses.clear();
    {
        vecProcesses = { };
    }

    vecCommandLines.clear();
    {
        vecCommandLines = { };
    }

    vecExePaths.clear();
    {
        vecExePaths = { };
    }

    if (::CoInitializeEx({ }, { }) || ::CoInitializeSecurity({ }, -1L, { }, { }, { }, 3UL, { }, { }, { }) ||
        ::CoCreateInstance(::CLSID_WbemLocator, { }, 1UL, ::IID_IWbemLocator, (void**)&Var.pWbemLocator) || !Var.pWbemLocator ||
        !(Var.pNetworkResourceSysString = ::SysAllocString(L"ROOT\\CIMV2")) ||
        Var.pWbemLocator->ConnectServer(Var.pNetworkResourceSysString, { }, { }, { }, { }, { }, { }, &Var.pWbemServices) || !Var.pWbemServices ||
        !(Var.pQueryLangSysString = ::SysAllocString(L"WQL")) ||
        !(Var.pQuerySysString = ::SysAllocString(L"SELECT ProcessId, CommandLine, ExecutablePath FROM Win32_Process")) ||
        Var.pWbemServices->ExecQuery(Var.pQueryLangSysString, Var.pQuerySysString, { }, { }, &Var.pEnumWbemClassObject) || !Var.pEnumWbemClassObject)
    {
        ::queryProcFreeData(Var);
        {
            Var = { };
        }

        ::CoUninitialize();

        return { };
    }

    _ = { };

    while (!Var.pEnumWbemClassObject->Next(-1L, 1UL, &Var.pWbemClassObject, &_))
    {
        if (Var.pWbemClassObject)
        {
            Var.pWbemClassObject->Get(L"ProcessId", { }, &Var.ProcessIndex, { }, { });
            vecProcesses.emplace_back(Var.ProcessIndex.ulVal);
            ::VariantClear(&Var.ProcessIndex);
            Var.ProcessIndex = { };

            Var.pWbemClassObject->Get(L"CommandLine", { }, &Var.ProcessCommandLine, { }, { });
            ::std::wstring pmStr(Var.ProcessCommandLine.bstrVal, ::SysStringLen(Var.ProcessCommandLine.bstrVal));
            vecCommandLines.emplace_back(::toMultiByte(pmStr));
            ::VariantClear(&Var.ProcessCommandLine);
            Var.ProcessCommandLine = { };

            Var.pWbemClassObject->Get(L"ExecutablePath", { }, &Var.ProcessExePath, { }, { });
            ::std::wstring phStr(Var.ProcessExePath.bstrVal, ::SysStringLen(Var.ProcessExePath.bstrVal));
            vecExePaths.emplace_back(::toMultiByte(phStr));
            ::VariantClear(&Var.ProcessExePath);
            Var.ProcessExePath = { };

            Var.pWbemClassObject->Release();
            {
                Var.pWbemClassObject = { };
            }
        }

        _ = { };
    }

    ::queryProcFreeData(Var);
    {
        Var = { };
    }

    ::CoUninitialize();

    return true;
}

bool queryProcesses(::std::vector < unsigned long >& vecProcesses, ::std::vector < ::std::wstring >& vecCommandLines, ::std::vector < ::std::wstring >& vecExePaths) noexcept
{
    static ::queryProcData Var{ };

    static unsigned long _{ };

    vecProcesses.clear();
    {
        vecProcesses = { };
    }

    vecCommandLines.clear();
    {
        vecCommandLines = { };
    }

    vecExePaths.clear();
    {
        vecExePaths = { };
    }

    if (::CoInitializeEx({ }, { }) || ::CoInitializeSecurity({ }, -1L, { }, { }, { }, 3UL, { }, { }, { }) ||
        ::CoCreateInstance(::CLSID_WbemLocator, { }, 1UL, ::IID_IWbemLocator, (void**)&Var.pWbemLocator) || !Var.pWbemLocator ||
        !(Var.pNetworkResourceSysString = ::SysAllocString(L"ROOT\\CIMV2")) ||
        Var.pWbemLocator->ConnectServer(Var.pNetworkResourceSysString, { }, { }, { }, { }, { }, { }, &Var.pWbemServices) || !Var.pWbemServices ||
        !(Var.pQueryLangSysString = ::SysAllocString(L"WQL")) ||
        !(Var.pQuerySysString = ::SysAllocString(L"SELECT ProcessId, CommandLine, ExecutablePath FROM Win32_Process")) ||
        Var.pWbemServices->ExecQuery(Var.pQueryLangSysString, Var.pQuerySysString, { }, { }, &Var.pEnumWbemClassObject) || !Var.pEnumWbemClassObject)
    {
        ::queryProcFreeData(Var);
        {
            Var = { };
        }

        ::CoUninitialize();

        return { };
    }

    _ = { };

    while (!Var.pEnumWbemClassObject->Next(-1L, 1UL, &Var.pWbemClassObject, &_))
    {
        if (Var.pWbemClassObject)
        {
            Var.pWbemClassObject->Get(L"ProcessId", { }, &Var.ProcessIndex, { }, { });
            vecProcesses.emplace_back(Var.ProcessIndex.ulVal);
            ::VariantClear(&Var.ProcessIndex);
            Var.ProcessIndex = { };

            Var.pWbemClassObject->Get(L"CommandLine", { }, &Var.ProcessCommandLine, { }, { });
            ::std::wstring pmStr(Var.ProcessCommandLine.bstrVal, ::SysStringLen(Var.ProcessCommandLine.bstrVal));
            vecCommandLines.emplace_back(pmStr);
            ::VariantClear(&Var.ProcessCommandLine);
            Var.ProcessCommandLine = { };

            Var.pWbemClassObject->Get(L"ExecutablePath", { }, &Var.ProcessExePath, { }, { });
            ::std::wstring phStr(Var.ProcessExePath.bstrVal, ::SysStringLen(Var.ProcessExePath.bstrVal));
            vecExePaths.emplace_back(phStr);
            ::VariantClear(&Var.ProcessExePath);
            Var.ProcessExePath = { };

            Var.pWbemClassObject->Release();
            {
                Var.pWbemClassObject = { };
            }
        }

        _ = { };
    }

    ::queryProcFreeData(Var);
    {
        Var = { };
    }

    ::CoUninitialize();

    return true;
}

::std::string toLwr(::std::string _) noexcept
{
    ::std::transform(_.begin(), _.end(), _.begin(), ::tolower);

    return _;
}

::std::wstring toLwr(::std::wstring _) noexcept
{
    ::std::transform(_.begin(), _.end(), _.begin(), ::towlower);

    return _;
}

::std::string toUpr(::std::string _) noexcept
{
    ::std::transform(_.begin(), _.end(), _.begin(), ::toupper);

    return _;
}

::std::wstring toUpr(::std::wstring _) noexcept
{
    ::std::transform(_.begin(), _.end(), _.begin(), ::towupper);

    return _;
}

::std::string readPath(::std::string _) noexcept
{
    static unsigned long long ullPos{ };

    if (_.empty() || _.ends_with('/') || _.ends_with('\\'))
    {
        return _;
    }

    else if ((ullPos = _.find_last_of('/')) != ::std::string::npos || (ullPos = _.find_last_of('\\')) != ::std::string::npos)
    {
        _.erase(ullPos + ((unsigned long long) (1I8)));
    }

    return _;
}

::std::wstring readPath(::std::wstring _) noexcept
{
    static unsigned long long ullPos{ };

    if (_.empty() || _.ends_with(L'/') || _.ends_with(L'\\'))
    {
        return _;
    }

    else if ((ullPos = _.find_last_of(L'/')) != ::std::wstring::npos || (ullPos = _.find_last_of(L'\\')) != ::std::wstring::npos)
    {
        _.erase(ullPos + ((unsigned long long) (1I8)));
    }

    return _;
}

unsigned long selfProcAlreadyRunning() noexcept
{
    static unsigned long ulProcs[2048I16]{ }, ulNum{ }, ulSelf{ }, ulIter{ };

    static char szSelf[512I16]{ }, szProc[512I16]{ };

    static void* pProc{ }, * pSelf{ };

    if (!(pSelf = ::GetCurrentProcess()) || !::K32GetModuleFileNameExA(pSelf, nullptr, szSelf, ((((unsigned long)(sizeof szSelf)) / ((unsigned long)(sizeof(decltype (*szSelf))))) - ((unsigned long)(1I8)))) || !::K32EnumProcesses(ulProcs, ((unsigned long)(sizeof ulProcs)), &ulNum) || ((ulSelf = ::GetCurrentProcessId()) < ((unsigned long)(1I8))))
    {
        return { };
    }

    for (ulIter = { }; ((ulIter) < ((ulNum) / ((unsigned long)(sizeof(decltype (*ulProcs)))))); ulIter++)
    {
        if (ulProcs[ulIter] > ((unsigned long)(0I8)))
        {
            if (ulProcs[ulIter] != ulSelf)
            {
                if (pProc = ::OpenProcess(2097151UL, { }, ulProcs[ulIter]))
                {
                    ::std::memset(szProc, { }, ((unsigned long long) (sizeof szProc)));
                    {
                        ::K32GetModuleFileNameExA(pProc, nullptr, szProc, ((((unsigned long)(sizeof szProc)) / ((unsigned long)(sizeof(decltype (*szProc))))) - ((unsigned long)(1I8))));
                        {
                            ::CloseHandle(pProc);
                            {
                                pProc = { };
                                {
                                    if (!(::lstrcmpiA(::toLwr(szProc).c_str(), ::toLwr(szSelf).c_str())))
                                    {
                                        return (ulProcs[ulIter]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return { };
}

unsigned long selfProcAlreadyRunningUnicode() noexcept
{
    static unsigned long ulProcs[2048I16]{ }, ulNum{ }, ulSelf{ }, ulIter{ };

    static wchar_t wszSelf[512I16]{ }, wszProc[512I16]{ };

    static void* pProc{ }, * pSelf{ };

    if (!(pSelf = ::GetCurrentProcess()) || !::K32GetModuleFileNameExW(pSelf, nullptr, wszSelf, ((((unsigned long)(sizeof wszSelf)) / ((unsigned long)(sizeof(decltype (*wszSelf))))) - ((unsigned long)(1I8)))) || !::K32EnumProcesses(ulProcs, ((unsigned long)(sizeof ulProcs)), &ulNum) || ((ulSelf = ::GetCurrentProcessId()) < ((unsigned long)(1I8))))
    {
        return { };
    }

    for (ulIter = { }; ((ulIter) < ((ulNum) / ((unsigned long)(sizeof(decltype (*ulProcs)))))); ulIter++)
    {
        if (ulProcs[ulIter] > ((unsigned long)(0I8)))
        {
            if (ulProcs[ulIter] != ulSelf)
            {
                if (pProc = ::OpenProcess(2097151UL, { }, ulProcs[ulIter]))
                {
                    ::std::memset(wszProc, { }, ((unsigned long long) (sizeof wszProc)));
                    {
                        ::K32GetModuleFileNameExW(pProc, nullptr, wszProc, ((((unsigned long)(sizeof wszProc)) / ((unsigned long)(sizeof(decltype (*wszProc))))) - ((unsigned long)(1I8))));
                        {
                            ::CloseHandle(pProc);
                            {
                                pProc = { };
                                {
                                    if (!(::lstrcmpiW(::toLwr(wszProc).c_str(), ::toLwr(wszSelf).c_str())))
                                    {
                                        return (ulProcs[ulIter]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return { };
}

unsigned long procRunning(::std::string strExePathNeedle) noexcept
{
    static unsigned long ulProcs[2048I16]{ }, ulNum{ }, ulSelf{ }, ulIter{ };

    static char szProc[512I16]{ };

    static void* pProc{ };

    if (strExePathNeedle.empty() || !::K32EnumProcesses(ulProcs, ((unsigned long)(sizeof ulProcs)), &ulNum) || ((ulSelf = ::GetCurrentProcessId()) < ((unsigned long)(1I8))))
    {
        return { };
    }

    for (ulIter = { }; ((ulIter) < ((ulNum) / ((unsigned long)(sizeof(decltype (*ulProcs)))))); ulIter++)
    {
        if (ulProcs[ulIter] > ((unsigned long)(0I8)))
        {
            if (ulProcs[ulIter] != ulSelf)
            {
                if (pProc = ::OpenProcess(2097151UL, { }, ulProcs[ulIter]))
                {
                    ::std::memset(szProc, { }, ((unsigned long long) (sizeof szProc)));
                    {
                        ::K32GetModuleFileNameExA(pProc, nullptr, szProc, ((((unsigned long)(sizeof szProc)) / ((unsigned long)(sizeof(decltype (*szProc))))) - ((unsigned long)(1I8))));
                        {
                            ::CloseHandle(pProc);
                            {
                                pProc = { };
                                {
                                    if (::strstr(::toLwr(szProc).c_str(), ::toLwr(strExePathNeedle).c_str()))
                                    {
                                        return (ulProcs[ulIter]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return { };
}

unsigned long procRunning(::std::wstring wstrExePathNeedle) noexcept
{
    static unsigned long ulProcs[2048I16]{ }, ulNum{ }, ulSelf{ }, ulIter{ };

    static wchar_t wszProc[512I16]{ };

    static void* pProc{ };

    if (wstrExePathNeedle.empty() || !::K32EnumProcesses(ulProcs, ((unsigned long)(sizeof ulProcs)), &ulNum) || ((ulSelf = ::GetCurrentProcessId()) < ((unsigned long)(1I8))))
    {
        return { };
    }

    for (ulIter = { }; ((ulIter) < ((ulNum) / ((unsigned long)(sizeof(decltype (*ulProcs)))))); ulIter++)
    {
        if (ulProcs[ulIter] > ((unsigned long)(0I8)))
        {
            if (ulProcs[ulIter] != ulSelf)
            {
                if (pProc = ::OpenProcess(2097151UL, { }, ulProcs[ulIter]))
                {
                    ::std::memset(wszProc, { }, ((unsigned long long) (sizeof wszProc)));
                    {
                        ::K32GetModuleFileNameExW(pProc, nullptr, wszProc, ((((unsigned long)(sizeof wszProc)) / ((unsigned long)(sizeof(decltype (*wszProc))))) - ((unsigned long)(1I8))));
                        {
                            ::CloseHandle(pProc);
                            {
                                pProc = { };
                                {
                                    if (::wcsstr(::toLwr(wszProc).c_str(), ::toLwr(wstrExePathNeedle).c_str()))
                                    {
                                        return (ulProcs[ulIter]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return { };
}

unsigned long runProc(::std::string strExePath, ::std::string strCmdLine, bool bOnlyIfDead) noexcept
{
    static ::STARTUPINFOA startUpInfo{ };
    static ::PROCESS_INFORMATION procInfo{ };

    static unsigned long ulRes{ };

    if (strExePath.empty() || strCmdLine.empty() || (bOnlyIfDead && (::procRunning(strExePath) || ::procRunning(::inverseSlashes(strExePath)))))
    {
        return { };
    }

    else
    {
        ::std::memset(&startUpInfo, { }, sizeof startUpInfo);
        {
            startUpInfo.cb = sizeof startUpInfo;
            {
                ::std::memset(&procInfo, { }, sizeof procInfo);
            }
        }
    }

    if (::CreateProcessA(strExePath.c_str(), ((char*)(strCmdLine.c_str())), { }, { }, { }, 144UL, { }, ::readPath(strExePath).c_str(), &startUpInfo, &procInfo))
    {
        if (procInfo.hThread)
        {
            ::CloseHandle(procInfo.hThread);
        }

        if (procInfo.hProcess)
        {
            ulRes = ::GetProcessId(procInfo.hProcess);
            {
                ::CloseHandle(procInfo.hProcess);
            }
        }

        else
        {
            ulRes = { };
        }

        return ulRes;
    }

    else
    {
        if (procInfo.hThread)
        {
            ::CloseHandle(procInfo.hThread);
        }

        if (procInfo.hProcess)
        {
            ::CloseHandle(procInfo.hProcess);
        }

        ::Sleep(::readProcRunInterval());
        {
            if (!(bOnlyIfDead && (::procRunning(strExePath) || ::procRunning(::inverseSlashes(strExePath)))))
            {
                if (!::readUsePipe())
                {
                    ::std::system(("start " + strCmdLine).c_str());
                }

                else
                {
                    ::_pclose(::_popen(("start " + strCmdLine).c_str(), "r"));
                }

                {
                    ::Sleep(1000UL);
                    {
                        if (::procRunning(strExePath))
                        {
                            return ::procRunning(strExePath);
                        }

                        else if (::procRunning(::inverseSlashes(strExePath)))
                        {
                            return ::procRunning(::inverseSlashes(strExePath));
                        }
                    }
                }
            }
        }
    }

    return { };
}

unsigned long runProc(::std::wstring wstrExePath, ::std::wstring wstrCmdLine, bool bOnlyIfDead) noexcept
{
    static ::STARTUPINFOW startUpInfo{ };
    static ::PROCESS_INFORMATION procInfo{ };

    static unsigned long ulRes{ };

    if (wstrExePath.empty() || wstrCmdLine.empty() || (bOnlyIfDead && (::procRunning(wstrExePath) || ::procRunning(::inverseSlashes(wstrExePath)))))
    {
        return { };
    }

    else
    {
        ::std::memset(&startUpInfo, { }, sizeof startUpInfo);
        {
            startUpInfo.cb = sizeof startUpInfo;
            {
                ::std::memset(&procInfo, { }, sizeof procInfo);
            }
        }
    }

    if (::CreateProcessW(wstrExePath.c_str(), ((wchar_t*)(wstrCmdLine.c_str())), { }, { }, { }, 144UL, { }, ::readPath(wstrExePath).c_str(), &startUpInfo, &procInfo))
    {
        if (procInfo.hThread)
        {
            ::CloseHandle(procInfo.hThread);
        }

        if (procInfo.hProcess)
        {
            ulRes = ::GetProcessId(procInfo.hProcess);
            {
                ::CloseHandle(procInfo.hProcess);
            }
        }

        else
        {
            ulRes = { };
        }

        return ulRes;
    }

    else
    {
        if (procInfo.hThread)
        {
            ::CloseHandle(procInfo.hThread);
        }

        if (procInfo.hProcess)
        {
            ::CloseHandle(procInfo.hProcess);
        }

        ::Sleep(::readProcRunInterval());
        {
            if (!(bOnlyIfDead && (::procRunning(wstrExePath) || ::procRunning(::inverseSlashes(wstrExePath)))))
            {
                if (!::readUsePipe())
                {
                    ::_wsystem((L"start " + wstrCmdLine).c_str());
                }

                else
                {
                    ::_pclose(::_wpopen((L"start " + wstrCmdLine).c_str(), L"r"));
                }

                {
                    ::Sleep(1000UL);
                    {
                        if (::procRunning(wstrExePath))
                        {
                            return ::procRunning(wstrExePath);
                        }

                        else if (::procRunning(::inverseSlashes(wstrExePath)))
                        {
                            return ::procRunning(::inverseSlashes(wstrExePath));
                        }
                    }
                }
            }
        }
    }

    return { };
}

::nlohmann::json readFile() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::fstream inFile{ };
    static ::std::string strPath{ };

    static unsigned long long ullPos{ };

    static char szPath[512I16]{ };

    if (!*szPath)
    {
        if (::GetModuleFileNameA(nullptr, szPath, ((((unsigned long)(sizeof szPath)) / ((unsigned long)(sizeof(decltype (*szPath))))) - ((unsigned long)(1I8)))))
        {
            strPath = szPath;
            {
                strPath.shrink_to_fit();
                {
                    if ((ullPos = strPath.find_last_of('/')) != ::std::string::npos)
                    {
                        strPath.erase(ullPos + ((unsigned long long) (1I8)));
                        {
                            strPath.shrink_to_fit();
                            {
                                strPath.append("config.js");
                                {
                                    strPath.shrink_to_fit();
                                }
                            }
                        }
                    }

                    else if ((ullPos = strPath.find_last_of('\\')) != ::std::string::npos)
                    {
                        strPath.erase(ullPos + ((unsigned long long) (1I8)));
                        {
                            strPath.shrink_to_fit();
                            {
                                strPath.append("config.js");
                                {
                                    strPath.shrink_to_fit();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (strPath.empty())
    {
        return jsTree;
    }

    inFile.open(strPath, ((int)(1I8)), ((int)(64I8)));

    if (inFile.is_open())
    {
        jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
        {
            inFile.close();
            {
                inFile.clear();
                {
                    return jsTree;
                }
            }
        }
    }

    else
    {
        inFile.clear();
    }

    return jsTree;
}

::nlohmann::json readFileUnicode() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::fstream inFile{ };
    static ::std::wstring wstrPath{ };

    static unsigned long long ullPos{ };

    static wchar_t wszPath[512I16]{ };

    if (!*wszPath)
    {
        if (::GetModuleFileNameW(nullptr, wszPath, ((((unsigned long)(sizeof wszPath)) / ((unsigned long)(sizeof(decltype (*wszPath))))) - ((unsigned long)(1I8)))))
        {
            wstrPath = wszPath;
            {
                wstrPath.shrink_to_fit();
                {
                    if ((ullPos = wstrPath.find_last_of(L'/')) != ::std::wstring::npos)
                    {
                        wstrPath.erase(ullPos + ((unsigned long long) (1I8)));
                        {
                            wstrPath.shrink_to_fit();
                            {
                                wstrPath.append(L"config.js");
                                {
                                    wstrPath.shrink_to_fit();
                                }
                            }
                        }
                    }

                    else if ((ullPos = wstrPath.find_last_of(L'\\')) != ::std::wstring::npos)
                    {
                        wstrPath.erase(ullPos + ((unsigned long long) (1I8)));
                        {
                            wstrPath.shrink_to_fit();
                            {
                                wstrPath.append(L"config.js");
                                {
                                    wstrPath.shrink_to_fit();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (wstrPath.empty())
    {
        return jsTree;
    }

    inFile.open(wstrPath, ((int)(1I8)), ((int)(64I8)));

    if (inFile.is_open())
    {
        jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
        {
            inFile.close();
            {
                inFile.clear();
                {
                    return jsTree;
                }
            }
        }
    }

    else
    {
        inFile.clear();
    }

    return jsTree;
}

unsigned long readLoopInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 10000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["LoopInterval"].is_discarded())
                            {
                                if (!jsTree["LoopInterval"].empty())
                                {
                                    if (jsTree["LoopInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["LoopInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readIntervalBetweenServicesAndProcesses() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 5000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["SvcProcInterval"].is_discarded())
                            {
                                if (!jsTree["SvcProcInterval"].empty())
                                {
                                    if (jsTree["SvcProcInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["SvcProcInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readActivityInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 30000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ActivityInterval"].is_discarded())
                            {
                                if (!jsTree["ActivityInterval"].empty())
                                {
                                    if (jsTree["ActivityInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["ActivityInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readKillInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 100UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["KillInterval"].is_discarded())
                            {
                                if (!jsTree["KillInterval"].empty())
                                {
                                    if (jsTree["KillInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["KillInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

bool readUsePipe() noexcept
{
    static ::nlohmann::json jsTree{ };

    static bool bRes{ false, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["UsePipe"].is_discarded())
                            {
                                if (!jsTree["UsePipe"].empty())
                                {
                                    if (jsTree["UsePipe"].is_boolean())
                                    {
                                        bRes = jsTree["UsePipe"].get < bool >();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return bRes;
}

unsigned long readProcKillInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 5000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ProcKillInterval"].is_discarded())
                            {
                                if (!jsTree["ProcKillInterval"].empty())
                                {
                                    if (jsTree["ProcKillInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["ProcKillInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readMaxHandles() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 6999UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["MaxHandles"].is_discarded())
                            {
                                if (!jsTree["MaxHandles"].empty())
                                {
                                    if (jsTree["MaxHandles"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["MaxHandles"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readMaxThreads() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 399UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["MaxThreads"].is_discarded())
                            {
                                if (!jsTree["MaxThreads"].empty())
                                {
                                    if (jsTree["MaxThreads"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["MaxThreads"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readRunInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 100UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["RunInterval"].is_discarded())
                            {
                                if (!jsTree["RunInterval"].empty())
                                {
                                    if (jsTree["RunInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["RunInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readProcRunInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 2000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ProcRunInterval"].is_discarded())
                            {
                                if (!jsTree["ProcRunInterval"].empty())
                                {
                                    if (jsTree["ProcRunInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["ProcRunInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readServicesInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 1800UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ServicesInterval"].is_discarded())
                            {
                                if (!jsTree["ServicesInterval"].empty())
                                {
                                    if (jsTree["ServicesInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["ServicesInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

unsigned long readKillRunInterval() noexcept
{
    static ::nlohmann::json jsTree{ };

    static unsigned long ulRes{ 5000UL, };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["KillRunInterval"].is_discarded())
                            {
                                if (!jsTree["KillRunInterval"].empty())
                                {
                                    if (jsTree["KillRunInterval"].is_number_unsigned())
                                    {
                                        ulRes = ((unsigned long)(jsTree["KillRunInterval"].get < unsigned int >()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ulRes;
}

::std::string readHideFileName() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::string strRes{ "nohideme", };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["HideFileName"].is_discarded())
                            {
                                if (!jsTree["HideFileName"].empty())
                                {
                                    if (jsTree["HideFileName"].is_string())
                                    {
                                        strVal = jsTree["HideFileName"].get < ::std::string >();
                                        {
                                            strVal.shrink_to_fit();
                                            {
                                                if (!(strVal.empty()))
                                                {
                                                    strRes = strVal;
                                                    {
                                                        strRes.shrink_to_fit();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return strRes;
}

::std::wstring readHideFileNameUnicode() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::wstring wstrRes{ L"nohideme", };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["HideFileName"].is_discarded())
                            {
                                if (!jsTree["HideFileName"].empty())
                                {
                                    if (jsTree["HideFileName"].is_string())
                                    {
                                        strVal = jsTree["HideFileName"].get < ::std::string >();
                                        {
                                            strVal.shrink_to_fit();
                                            {
                                                if (!(strVal.empty()))
                                                {
                                                    wstrRes = ::toUnicode(strVal);
                                                    {
                                                        wstrRes.shrink_to_fit();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return wstrRes;
}

::std::vector < ::std::string > readServerProcessNames() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::string strName{ };

    static ::std::vector < ::std::string > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ServerProcessNames"].is_discarded())
                            {
                                if (!jsTree["ServerProcessNames"].empty())
                                {
                                    if (jsTree["ServerProcessNames"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["ServerProcessNames"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    strName.assign(strVal);
                                                                    {
                                                                        strName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(strName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::vector < ::std::wstring > readServerProcessNamesUnicode() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::wstring wstrName{ };

    static ::std::vector < ::std::wstring > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ServerProcessNames"].is_discarded())
                            {
                                if (!jsTree["ServerProcessNames"].empty())
                                {
                                    if (jsTree["ServerProcessNames"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["ServerProcessNames"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    wstrName.assign(::toUnicode(strVal));
                                                                    {
                                                                        wstrName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(wstrName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::vector < ::std::string > readServiceProcessNames() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::string strName{ };

    static ::std::vector < ::std::string > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ServiceProcessNames"].is_discarded())
                            {
                                if (!jsTree["ServiceProcessNames"].empty())
                                {
                                    if (jsTree["ServiceProcessNames"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["ServiceProcessNames"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    strName.assign(strVal);
                                                                    {
                                                                        strName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(strName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::vector < ::std::wstring > readServiceProcessNamesUnicode() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::wstring wstrName{ };

    static ::std::vector < ::std::wstring > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["ServiceProcessNames"].is_discarded())
                            {
                                if (!jsTree["ServiceProcessNames"].empty())
                                {
                                    if (jsTree["ServiceProcessNames"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["ServiceProcessNames"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    wstrName.assign(::toUnicode(strVal));
                                                                    {
                                                                        wstrName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(wstrName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::vector < ::std::string > readTaskKillNames() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::string strName{ };

    static ::std::vector < ::std::string > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["RunTaskKill"].is_discarded())
                            {
                                if (!jsTree["RunTaskKill"].empty())
                                {
                                    if (jsTree["RunTaskKill"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["RunTaskKill"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    strName.assign(strVal);
                                                                    {
                                                                        strName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(strName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::vector < ::std::wstring > readTaskKillNamesUnicode() noexcept
{
    static ::nlohmann::json jsTree{ };

    static ::std::string strVal{ };
    static ::std::wstring wstrName{ };

    static ::std::vector < ::std::wstring > vecRes{ };

    static long long llNow{ }, llStamp{ };

    if ((llNow = ::std::time(nullptr)) > llStamp)
    {
        llStamp = llNow + ((long long)(180I16));
        {
            jsTree = ::readFileUnicode();
            {
                if (!jsTree.is_discarded())
                {
                    if (!jsTree.empty())
                    {
                        if (jsTree.is_object())
                        {
                            if (!jsTree["RunTaskKill"].is_discarded())
                            {
                                if (!jsTree["RunTaskKill"].empty())
                                {
                                    if (jsTree["RunTaskKill"].is_array())
                                    {
                                        vecRes.clear();
                                        {
                                            vecRes.shrink_to_fit();
                                            {
                                                for (auto& jsName : jsTree["RunTaskKill"])
                                                {
                                                    if (!(jsName.empty()))
                                                    {
                                                        strVal = jsName.get < ::std::string >();
                                                        {
                                                            strVal.shrink_to_fit();
                                                            {
                                                                if (!(strVal.empty()))
                                                                {
                                                                    wstrName.assign(::toUnicode(strVal));
                                                                    {
                                                                        wstrName.shrink_to_fit();
                                                                        {
                                                                            vecRes.push_back(wstrName);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                vecRes.shrink_to_fit();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return vecRes;
}

::std::string serviceByNeedle(::std::string strNeedle) noexcept
{
    static ::std::string strRes{ };

    static ::SC_HANDLE__* pManager{ };

    static ::_ENUM_SERVICE_STATUS_PROCESSA esspServices[4096I16]{ };

    static unsigned long ulBytesNeeded{ }, ulServicesReturned{ }, ulResumeHandle{ }, ulIter{ };

    if (strNeedle.empty())
    {
        return { };
    }

    strRes.clear();
    {
        strRes.shrink_to_fit();
        {
            ulBytesNeeded = ulServicesReturned = ulResumeHandle = ulIter = { };
            {
                pManager = { };
                {
                    ::std::memset(esspServices, { }, ((unsigned long long) (sizeof esspServices)));
                    {
                        if (!(strNeedle.empty()))
                        {
                            if (pManager = ::OpenSCManagerA(nullptr, "ServicesActive", 983103UL))
                            {
                                if (::EnumServicesStatusExA(pManager, { }, ((unsigned long)(16I8)), ((unsigned long)(3I8)), ((unsigned char*)(esspServices)), ((unsigned long)(sizeof esspServices)), &ulBytesNeeded, &ulServicesReturned, &ulResumeHandle, nullptr))
                                {
                                    for (ulIter = { }; ulIter < ulServicesReturned; ulIter++)
                                    {
                                        if (esspServices[ulIter].lpServiceName)
                                        {
                                            if (::std::strstr(::toLwr(esspServices[ulIter].lpServiceName).c_str(), ::toLwr(strNeedle).c_str()))
                                            {
                                                strRes.assign(esspServices[ulIter].lpServiceName);
                                                {
                                                    strRes.shrink_to_fit();
                                                    {
                                                        ::CloseServiceHandle(pManager);
                                                        {
                                                            pManager = { };
                                                            {
                                                                return strRes;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                ::CloseServiceHandle(pManager);
                                {
                                    pManager = { };
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return strRes;
}

::std::wstring serviceByNeedle(::std::wstring wstrNeedle) noexcept
{
    static ::std::wstring wstrRes{ };

    static ::SC_HANDLE__* pManager{ };

    static ::_ENUM_SERVICE_STATUS_PROCESSW esspServices[4096I16]{ };

    static unsigned long ulBytesNeeded{ }, ulServicesReturned{ }, ulResumeHandle{ }, ulIter{ };

    if (wstrNeedle.empty())
    {
        return { };
    }

    wstrRes.clear();
    {
        wstrRes.shrink_to_fit();
        {
            ulBytesNeeded = ulServicesReturned = ulResumeHandle = ulIter = { };
            {
                pManager = { };
                {
                    ::std::memset(esspServices, { }, ((unsigned long long) (sizeof esspServices)));
                    {
                        if (!(wstrNeedle.empty()))
                        {
                            if (pManager = ::OpenSCManagerW(nullptr, L"ServicesActive", 983103UL))
                            {
                                if (::EnumServicesStatusExW(pManager, { }, ((unsigned long)(16I8)), ((unsigned long)(3I8)), ((unsigned char*)(esspServices)), ((unsigned long)(sizeof esspServices)), &ulBytesNeeded, &ulServicesReturned, &ulResumeHandle, nullptr))
                                {
                                    for (ulIter = { }; ulIter < ulServicesReturned; ulIter++)
                                    {
                                        if (esspServices[ulIter].lpServiceName)
                                        {
                                            if (::std::wcsstr(::toLwr(esspServices[ulIter].lpServiceName).c_str(), ::toLwr(wstrNeedle).c_str()))
                                            {
                                                wstrRes.assign(esspServices[ulIter].lpServiceName);
                                                {
                                                    wstrRes.shrink_to_fit();
                                                    {
                                                        ::CloseServiceHandle(pManager);
                                                        {
                                                            pManager = { };
                                                            {
                                                                return wstrRes;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                ::CloseServiceHandle(pManager);
                                {
                                    pManager = { };
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return wstrRes;
}

void computeProcessesA() noexcept
{
    static unsigned long ulIter{ }, ulProcHandles{ }, ulSelfProcIndex{ };

    static void* pProc{ }, * pSnap{ };

    static ::tagTHREADENTRY32 threadEntry{ };

    static ::std::string strIndex{ };

    static ::std::vector < unsigned long > vecProcesses{ };

    static ::std::vector < ::std::string > vecProcCmdLines{ };
    static ::std::vector < ::std::string > vecProcExePaths{ };

    static ::std::unordered_map < ::std::string, unsigned long > vecProcThreads{ };

    if (::queryProcesses(vecProcesses, vecProcCmdLines, vecProcExePaths))
    {
        vecProcThreads.clear();
        {
            if (pSnap = ::CreateToolhelp32Snapshot(4UL, { }))
            {
                ::std::memset(&threadEntry, { }, sizeof threadEntry);
                {
                    threadEntry.dwSize = sizeof threadEntry;
                }

                if (::Thread32First(pSnap, &threadEntry))
                {
                    do
                    {
                        if (threadEntry.th32OwnerProcessID > ((unsigned long)(0I8)))
                        {
                            vecProcThreads[::std::to_string(threadEntry.th32OwnerProcessID)]++;
                        }
                    }

                    while (::Thread32Next(pSnap, &threadEntry));
                }

                ::CloseHandle(pSnap);
                {
                    pSnap = { };
                }
            }
        }

        ulSelfProcIndex = ::GetCurrentProcessId();
        {
            ulIter = { };
        }

        for (auto& _ : vecProcesses)
        {
            if (ulSelfProcIndex != vecProcesses[ulIter])
            {
                if (vecProcesses[ulIter] > ((unsigned long)(0I8)))
                {
                    if (pProc = ::OpenProcess(2097151UL, { }, vecProcesses[ulIter]))
                    {
                        ulProcHandles = { };
                        {
                            ::GetProcessHandleCount(pProc, &ulProcHandles);
                        }

                        if (ulProcHandles > ::readMaxHandles())
                        {
                            ::TerminateProcess(pProc, { });
                            {
                                ::std::cout << "Terminated A Process (Handles: " << ulProcHandles << ")" << ::std::endl << "Proc Index: " << vecProcesses[ulIter] << ::std::endl <<
                                    "Executable Path: " << vecProcExePaths[ulIter] << ::std::endl << "Command Line: " << vecProcCmdLines[ulIter] << ::std::endl << ::std::endl;

                                ::Sleep(::readProcKillInterval());
                                {
                                    ::runProc(vecProcExePaths[ulIter], vecProcCmdLines[ulIter], true);
                                    {
                                        ::Sleep(::readProcRunInterval());
                                    }
                                }
                            }
                        }

                        else if (!vecProcThreads.empty())
                        {
                            if (vecProcThreads[strIndex = ::std::to_string(vecProcesses[ulIter])] > ::readMaxThreads())
                            {
                                ::TerminateProcess(pProc, { });
                                {
                                    ::std::cout << "Terminated A Process (Threads: " << vecProcThreads[strIndex] << ")" << ::std::endl << "Proc Index: " << vecProcesses[ulIter] <<
                                        ::std::endl << "Executable Path: " << vecProcExePaths[ulIter] << ::std::endl << "Command Line: " << vecProcCmdLines[ulIter] << ::std::endl <<
                                        ::std::endl;

                                    ::Sleep(::readProcKillInterval());
                                    {
                                        ::runProc(vecProcExePaths[ulIter], vecProcCmdLines[ulIter], true);
                                        {
                                            ::Sleep(::readProcRunInterval());
                                        }
                                    }
                                }
                            }
                        }

                        ::CloseHandle(pProc);
                    }
                }
            }

            ulIter++;
        }
    }
}

void computeProcessesW() noexcept
{
    static unsigned long ulIter{ }, ulProcHandles{ }, ulSelfProcIndex{ };

    static void* pProc{ }, * pSnap{ };

    static ::tagTHREADENTRY32 threadEntry{ };

    static ::std::string strIndex{ };

    static ::std::vector < unsigned long > vecProcesses{ };

    static ::std::vector < ::std::wstring > vecProcCmdLines{ };
    static ::std::vector < ::std::wstring > vecProcExePaths{ };

    static ::std::unordered_map < ::std::string, unsigned long > vecProcThreads{ };

    if (::queryProcesses(vecProcesses, vecProcCmdLines, vecProcExePaths))
    {
        vecProcThreads.clear();
        {
            if (pSnap = ::CreateToolhelp32Snapshot(4UL, { }))
            {
                ::std::memset(&threadEntry, { }, sizeof threadEntry);
                {
                    threadEntry.dwSize = sizeof threadEntry;
                }

                if (::Thread32First(pSnap, &threadEntry))
                {
                    do
                    {
                        if (threadEntry.th32OwnerProcessID > ((unsigned long)(0I8)))
                        {
                            vecProcThreads[::std::to_string(threadEntry.th32OwnerProcessID)]++;
                        }
                    }

                    while (::Thread32Next(pSnap, &threadEntry));
                }

                ::CloseHandle(pSnap);
                {
                    pSnap = { };
                }
            }
        }

        ulSelfProcIndex = ::GetCurrentProcessId();
        {
            ulIter = { };
        }

        for (auto& _ : vecProcesses)
        {
            if (ulSelfProcIndex != vecProcesses[ulIter])
            {
                if (vecProcesses[ulIter] > ((unsigned long)(0I8)))
                {
                    if (pProc = ::OpenProcess(2097151UL, { }, vecProcesses[ulIter]))
                    {
                        ulProcHandles = { };
                        {
                            ::GetProcessHandleCount(pProc, &ulProcHandles);
                        }

                        if (ulProcHandles > ::readMaxHandles())
                        {
                            ::TerminateProcess(pProc, { });
                            {
                                ::std::wcout << L"Terminated A Process (Handles: " << ulProcHandles << L")" << ::std::endl << L"Proc Index: " << vecProcesses[ulIter] << ::std::endl <<
                                    L"Executable Path: " << vecProcExePaths[ulIter] << ::std::endl << L"Command Line: " << vecProcCmdLines[ulIter] << ::std::endl << ::std::endl;

                                ::Sleep(::readProcKillInterval());
                                {
                                    ::runProc(vecProcExePaths[ulIter], vecProcCmdLines[ulIter], true);
                                    {
                                        ::Sleep(::readProcRunInterval());
                                    }
                                }
                            }
                        }

                        else if (!vecProcThreads.empty())
                        {
                            if (vecProcThreads[strIndex = ::std::to_string(vecProcesses[ulIter])] > ::readMaxThreads())
                            {
                                ::TerminateProcess(pProc, { });
                                {
                                    ::std::wcout << L"Terminated A Process (Threads: " << vecProcThreads[strIndex] << L")" << ::std::endl << L"Proc Index: " << vecProcesses[ulIter] <<
                                        ::std::endl << L"Executable Path: " << vecProcExePaths[ulIter] << ::std::endl << L"Command Line: " << vecProcCmdLines[ulIter] << ::std::endl <<
                                        ::std::endl;

                                    ::Sleep(::readProcKillInterval());
                                    {
                                        ::runProc(vecProcExePaths[ulIter], vecProcCmdLines[ulIter], true);
                                        {
                                            ::Sleep(::readProcRunInterval());
                                        }
                                    }
                                }
                            }
                        }

                        ::CloseHandle(pProc);
                    }
                }
            }

            ulIter++;
        }
    }
}

int __stdcall hideGamingServers(::HWND__* pWnd, long long llCurProc) noexcept
{
    static ::std::wstring wstrBuffer{ };

    static wchar_t wszStr[512I16]{ };

    static unsigned long ulProc{ };
    static unsigned long long ullPos{ };

    static void* pProc{ };

    if (pWnd)
    {
        ::GetWindowThreadProcessId(pWnd, &ulProc);
        {
            if (ulProc > ((unsigned long)(0I8)))
            {
                if (ulProc != ((unsigned long)(llCurProc)))
                {
                    if (pProc = ::OpenProcess(2097151UL, { }, ulProc))
                    {
                        ::std::memset(wszStr, { }, ((unsigned long long) (sizeof wszStr)));
                        {
                            ::K32GetModuleFileNameExW(pProc, nullptr, wszStr, ((((unsigned long)(sizeof wszStr)) / ((unsigned long)(sizeof(decltype (*wszStr))))) - ((unsigned long)(1I8))));
                            {
                                for (auto& wstrName : ::readServerProcessNamesUnicode())
                                {
                                    if (::std::wcsstr(::toLwr(wszStr).c_str(), ::toLwr(wstrName).c_str()) || ::std::wcsstr(::toLwr(wszStr).c_str(), ::toLwr(::inverseSlashes(wstrName)).c_str()))
                                    {
                                        wstrBuffer = wszStr;
                                        {
                                            wstrBuffer.shrink_to_fit();
                                            {
                                                if (!(wstrBuffer.empty()))
                                                {
                                                    if ((ullPos = wstrBuffer.find_last_of(L'/')) != ::std::wstring::npos)
                                                    {
                                                        wstrBuffer.erase(ullPos + ((unsigned long long) (1I8)));
                                                        {
                                                            wstrBuffer.shrink_to_fit();
                                                            {
                                                                wstrBuffer.append(::readHideFileNameUnicode());
                                                                {
                                                                    wstrBuffer.shrink_to_fit();
                                                                    {
                                                                        if (!::std::filesystem::exists(wstrBuffer))
                                                                        {
                                                                            ::ShowWindow(pWnd, { });
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    else if ((ullPos = wstrBuffer.find_last_of(L'\\')) != ::std::wstring::npos)
                                                    {
                                                        wstrBuffer.erase(ullPos + ((unsigned long long) (1I8)));
                                                        {
                                                            wstrBuffer.shrink_to_fit();
                                                            {
                                                                wstrBuffer.append(::readHideFileNameUnicode());
                                                                {
                                                                    wstrBuffer.shrink_to_fit();
                                                                    {
                                                                        if (!::std::filesystem::exists(wstrBuffer))
                                                                        {
                                                                            ::ShowWindow(pWnd, { });
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        ::CloseHandle(pProc);
                        {
                            pProc = { };
                        }
                    }
                }
            }
        }
    }

    return ((int)(1I8));
}

int __cdecl main() noexcept
{
    if (::selfProcAlreadyRunningUnicode())
    {
        return ((int)(1I8));
    }

    if (!::GetConsoleWindow())
    {
        ::AllocConsole();
    }

    if (::GetConsoleWindow())
    {
        ::SetConsoleTitleW(L"Bad Processes Manager (x64)");
    }

    ::std::wcout << L"WELCOME" << ::std::endl << ::std::endl;

    wchar_t wszStr[512I16]{ };
    {
        ::GetModuleFileNameW(nullptr, wszStr, ((((unsigned long)(sizeof wszStr)) / ((unsigned long)(sizeof(decltype (*wszStr))))) - ((unsigned long)(1I8))));
    }

    ::HKEY__* pKey{ };
    {
        if (::RegOpenKeyExW(((::HKEY__*)(2147483649ULL)), L"software\\microsoft\\windows\\currentversion\\run", { }, 983103UL, &pKey) || !pKey)
        {
            if (pKey)
            {
                ::RegCloseKey(pKey);
                {
                    pKey = { };
                }
            }

            ::std::wcout << L"RegOpenKeyExW Failed" << ::std::endl;
            {
                return ((int)(1I8));
            }
        }

        if (pKey)
        {
            ::RegSetValueExW(pKey, L"BadProcessesManager", { }, ((unsigned long)(1I8)), ((unsigned char*)(wszStr)), (((unsigned long)(sizeof(decltype (*wszStr)))) * (((unsigned long)(::std::wcslen(wszStr))) + ((unsigned long)(1I8)))));
            {
                ::RegCloseKey(pKey);
                {
                    pKey = { };
                }
            }
        }
    }

    long long llCurProc = ((long long)(::GetCurrentProcessId())), llNow{ }, llStamp{ };
    {
        ::tagLASTINPUTINFO liiData{ };
        {
            ::std::wstring wstrCommand{ }, wstrService{ };
            {
                while (true)
                {
                    ::std::memset(&liiData, { }, ((unsigned long long) (sizeof liiData)));
                    {
                        liiData.cbSize = ((unsigned int)(sizeof liiData));
                        {
                            if (::GetLastInputInfo(&liiData))
                            {
                                if ((::GetTickCount64() - liiData.dwTime) > (::readActivityInterval()))
                                {
                                    ::EnumWindows(::hideGamingServers, llCurProc);
                                    {
                                        if ((llNow = ::std::time(nullptr)) > llStamp)
                                        {
                                            llStamp = llNow + ((long long)(::readServicesInterval()));
                                            {
                                                for (auto& wstrName : ::readServiceProcessNamesUnicode())
                                                {
                                                    wstrService = ::serviceByNeedle(wstrName);
                                                    {
                                                        wstrService.shrink_to_fit();
                                                        {
                                                            if (!(wstrService.empty()))
                                                            {
                                                                wstrCommand = L"net stop \"" + wstrService + L"\"";
                                                                {
                                                                    wstrCommand.shrink_to_fit();
                                                                    {
                                                                        ::_wsystem(wstrCommand.c_str());
                                                                        {
                                                                            ::Sleep(::readKillInterval());
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                ::Sleep(::readKillRunInterval());
                                                {
                                                    for (auto& wstrName : ::readServiceProcessNamesUnicode())
                                                    {
                                                        wstrService = ::serviceByNeedle(wstrName);
                                                        {
                                                            wstrService.shrink_to_fit();
                                                            {
                                                                if (!(wstrService.empty()))
                                                                {
                                                                    wstrCommand = L"net start \"" + wstrService + L"\"";
                                                                    {
                                                                        wstrCommand.shrink_to_fit();
                                                                        {
                                                                            ::_wsystem(wstrCommand.c_str());
                                                                            {
                                                                                ::Sleep(::readRunInterval());
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    ::Sleep(::readIntervalBetweenServicesAndProcesses());
                                                    {
                                                        ::computeProcessesW();
                                                        {
                                                            ::Sleep(1000UL);
                                                            {
                                                                for (auto& wstrName : ::readTaskKillNamesUnicode())
                                                                {
                                                                    wstrCommand = L"taskkill /f /im \"" + wstrName + L"\"";
                                                                    {
                                                                        wstrCommand.shrink_to_fit();
                                                                        {
                                                                            ::_wsystem(wstrCommand.c_str());
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    ::Sleep(::readLoopInterval());
                }
            }
        }
    }

    return ((int)(0I8));
}
