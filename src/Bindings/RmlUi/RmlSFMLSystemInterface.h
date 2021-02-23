/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 Nuno Silva
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef SYSTEMINTEFACESFML_H
#define SYSTEMINTEFACESFML_H

#include <RmlUi/Core/SystemInterface.h>
#include <RmlUi/Core/Input.h>
#include <SFML/Graphics.hpp>

// TODO: This is primarily copy pasted from RmlUi git, The interface isn't great so a rewrite is in order.
//		 A bunch of functionality is also un-implemented. Such as cursors.
class RmlSFMLSystemInterface : public Rml::SystemInterface
{
public:

	RmlSFMLSystemInterface(sf::Window* pWindow);

	Rml::Input::KeyIdentifier TranslateKey(sf::Keyboard::Key Key);
	int GetKeyModifiers();

	void SetMouseCursor(const Rml::String& cursor) override;

	// Adding a cursor this way will assume ownership of the pointer.
	void AddCursor(const Rml::String& cursorName, sf::Cursor* pCursor);
	void AddSystemCursor(const Rml::String& cursorName, const sf::Cursor::Type type);
	
	double GetElapsedTime() override;
	bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

private:


	sf::Window* m_pWindow;
	sf::Clock m_timer;

	using CursorMap = Rml::UnorderedMap<Rml::String, sf::Cursor*>;
	CursorMap m_cursors;
};
#endif
