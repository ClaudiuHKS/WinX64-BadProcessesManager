
{
    //
    /// Wait X Milliseconds Between The User Activity Checks
    //
    "LoopInterval" :        10000,

    //
    /// The User Must Be Inactive For X Milliseconds To Perform Actions
    //
    "ActivityInterval" :    30000,

    //
    /// Only Kill And Run (Respawn) The Services Once In X Seconds
    //
    "ServicesInterval" :    1800,

    //
    /// After Each Service Kill Wait X Milliseconds
    //
    "KillInterval" :        100,

    //
    /// After Each Service Run Wait X Milliseconds
    //
    "RunInterval" :         100,

    //
    /// Wait X Milliseconds After Killing All The Desired Services Right Before Running Them Again
    //
    "KillRunInterval" :     5000,

    //
    /// If This File Ever Exists Into The Same Directory With The ["ServerProcessNames"] Executable That Game Server (Or Application) Will Not Go Hidden
    /// Will Also Not Go Visible If Already Hidden And The File Below Exists There
    /// In Windows Is Case Insensitive
    //
    "HideFileName" :        "nohideme",

    //
    /// Hide These Applications Below Depending On The Setting Above
    /// They Will Still Be Visible Into The Task Manager Application
    /// Case Sensitive
    /// It Probably Also Supports Full Paths So You Can Use / Or \ (For \ Use \\)
    /// If The Full Paths Are Supported Then lds.exe Will Be Positive For hlds.exe But Not For hlds.Exe
    //
    "ServerProcessNames" :
    [
        "hlds.exe",
        "srcds.exe"
    ],

    //
    /// Kill These Services One By One And Then Run Them Again One By One
    /// By Case Insensitive Part Of Name
    //
    "ServiceProcessNames" :
    [
        "SessionEnv",       /// Remote Desktop Configuration
        "UmRdpService",     /// Remote Desktop Services UserMode Port Redirector
        "TermService",      /// Remote Desktop Services

        "cbdhsvc_"          /// Clipboard User Service_
    ]
}
