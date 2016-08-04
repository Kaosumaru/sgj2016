#ifndef BHGAMEINITIALIZER
#define BHGAMEINITIALIZER



namespace BH
{
    

    
class GameInitializer 
{
public:
	static void Init();
	static void AfterScriptParse();
    static void ReloadScripts(bool reset = false);
};
    
}


#endif
