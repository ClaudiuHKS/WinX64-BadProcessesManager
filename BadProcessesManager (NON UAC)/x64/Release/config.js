
{
    //
    /// Wait X Milliseconds Between The User Activity Checks (1 Second = 1000 Milliseconds)
    //
    "LoopInterval" :        10000,

    //
    /// The User Must Be Inactive For X Milliseconds To Perform Actions
    //
    "ActivityInterval" :    30000,

    //
    /// Only Kill And Run (Respawn) The Services & Processes [Depending On Their Handles Count/ Threads Count] Once In X Seconds
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
    /// Kill Processes Using More Threads Than & Restart Them Afterwards (Use A Huge Value To Excluded Bad Processes Termination By Threads Count)
    //
    "MaxThreads" :          399,

    //
    /// Kill Processes Using More Handles Than & Restart Them Afterwards (Use A Huge Value To Excluded Bad Processes Termination By Handles Count)
    //
    "MaxHandles" :          6999,

    //
    /// Wait X Milliseconds Between The Services Computation And The Processes Computation (The Services Are Computed First)
    //
    "SvcProcInterval" :     5000,

    //
    /// Wait X Milliseconds Right After Terminating A Process (Not A Service)
    //
    "ProcKillInterval" :    5000,

    //
    /// Wait X Milliseconds Right After Starting A Process [After/ Once It Was Killed] (Not A Service)
    //
    "ProcRunInterval" :     2000,

    //
    /// If Pipe Is Used We Call `Pclose(Popen("start \"c:/path/to/process.exe\" -key=value", "r"))` Instead Of `System("start \"c:/path/to/process.exe\" -key=value")`
    //
    "UsePipe" :             false,

    //
    /// Hide These Applications Below Depending On The Setting Above ("HideFileName")
    /// They Will Still Be Visible Into The Task Manager Application
    /// Case Insensitive
    /// It Also Supports Full Paths So You Can Use / Or \ (For \ Use \\)
    //
    "ServerProcessNames" :
    [
        "/hlds.exe",
        "/srcds.exe"
    ],

    //
    /// Run Task Kill For These Processes (Useful If They Are Also Services) [See "ServicesInterval" Option]
    //
    "RunTaskKill" :
    [
        "GameManagerService.exe"
    ],

    //
    /// Kill These Services One By One And Then Run Them Again One By One
    /// By Case Insensitive Part Of Name
    //
    "ServiceProcessNames" :
    [
        "sessionenv",       /// Remote Desktop Configuration
        "umrdpservice",     /// Remote Desktop Services UserMode Port Redirector
        "termservice",      /// Remote Desktop Services

        "NVDisplay.ContainerLocalSystem",

        "cbdhsvc_"          /// Clipboard User Service_
    ]
}
