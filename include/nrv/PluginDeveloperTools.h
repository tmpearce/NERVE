#define M_MAKE_EXPORT_PLUGIN(FactoryClass) \
extern "C" __declspec(dllexport) void __cdecl RegisterNervePlugin(PluginRegistry * registry) \
{ \
	NerveAPI::registerWithApplication(new FactoryClass(),registry); \
} \

#define M_MAKE_FACTORY_CLASS(FactoryClass) \
class FactoryClass : public NervePluginFactory \
{ \
public: \
	NervePluginBase* createPluginObject(NerveAPI*); \
	void cleanUpPluginObject(NervePluginBase*,NerveAPI*); \
	const char* getName(); \
	void selfCleanup(){delete this;} \
};

#define M_MAKE_PLUGIN(FactoryClass) \
	M_MAKE_FACTORY_CLASS(FactoryClass) \
	M_MAKE_EXPORT_PLUGIN(FactoryClass)