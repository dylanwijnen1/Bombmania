#pragma once

namespace net
{
	class net_initializer
	{
	public:
		/// <summary>
		/// Can be called multiple times in order to check if the library is initialized.
		/// </summary>
		/// <returns></returns>
		static bool initialize();

	private:
		struct init_data;
	};
}