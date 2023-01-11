
#include < nlohmann/json.hpp >

#include < iostream >
#include < fstream >

#include < winsock2.h >
#include < psapi.h >
#include < wbemidl.h >
#include < windows.h >

::std::wstring toUnicode(::std::string _) noexcept
{
    static wchar_t wszStr[1024I16]{ };
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
    static char szStr[1024I16]{ };
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
                                    if (!(::lstrcmpiA(szProc, szSelf)))
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
                                    if (!(::lstrcmpiW(wszProc, wszSelf)))
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

::std::string serviceByNeedle(::std::string strNeedle) noexcept
{
    static ::std::string strRes{ };

    static ::SC_HANDLE__* pManager{ };

    static ::_ENUM_SERVICE_STATUS_PROCESSA esspServices[4096I16]{ };

    static unsigned long ulBytesNeeded{ }, ulServicesReturned{ }, ulResumeHandle{ }, ulIter{ };

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
                                            if (::std::strstr(esspServices[ulIter].lpServiceName, strNeedle.c_str()))
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
                                            if (::std::wcsstr(esspServices[ulIter].lpServiceName, wstrNeedle.c_str()))
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
                                    if (::std::wcsstr(wszStr, wstrName.c_str()))
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
                                                                wstrCommand = L"net stop " + wstrService;
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
                                                                    wstrCommand = L"net start " + wstrService;
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
