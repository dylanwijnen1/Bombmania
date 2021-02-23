namespace Rml
{
	class Context;
}

namespace Bombmania::UI
{
	class IController
	{
	public:

		virtual ~IController() = default;

		/// <summary>
		/// Bind the data model and controller functionality to the given model data name.
		/// </summary>
		/// <param name="pContext"></param>
		/// <param name="pName"></param>
		virtual void BindDataModel(Rml::Context* pContext, const char* pName) = 0;
	};
}