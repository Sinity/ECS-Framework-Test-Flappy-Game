#include "InputEcho.h"
#include <ECS/utils/loggerConsoleOutput.h>
#include "events/system/ApplicationClosed.h"
#include "events/system/KeyPressed.h"
#include "events/system/KeyReleased.h"
#include "events/system/MouseButtonPressed.h"
#include "events/system/MouseButtonReleased.h"
#include "events/system/MouseMoved.h"
#include "events/system/MouseWheelMoved.h"
#include "events/system/TextEntered.h"
#include "events/system/UnknownSFMLEvent.h"

InputEcho::InputEcho(Engine& engine) :
		Task(engine),
		debugLogger("DEBUG") {

	engine.events.connect<UnknownSFMLEvent>(*this);
	engine.events.connect<ApplicationClosedEvent>(*this);
	engine.events.connect<KeyPressed>(*this);
	engine.events.connect<KeyReleased>(*this);
	engine.events.connect<TextEntered>(*this);
	engine.events.connect<MouseButtonPressed>(*this);
	engine.events.connect<MouseButtonReleased>(*this);
	engine.events.connect<MouseMoved>(*this);
	engine.events.connect<MouseWheelMoved>(*this);

	if(engine.config.get("tasks.debugTask.log") == "true") {
		auto cOut = std::make_shared<ConsoleOutput>();
		cOut->setMinPriority(LogType::Information);
		debugLogger.addOutput(std::move(cOut));
	}
}

void InputEcho::update() {
}

void InputEcho::receive(UnknownSFMLEvent& unknownSFMLEvent) {
	debugLogger.info("Unknown sfml event{type=", (unsigned int)unknownSFMLEvent.event.type, "}");
}

void InputEcho::receive(ApplicationClosedEvent& appClosed) {
	(void)appClosed;
	debugLogger.info("Application closed event received");
}

void InputEcho::receive(KeyPressed& keyPressed) {
	debugLogger.info("Pressed key '", (unsigned int)keyPressed.key.code, "'{alt=", keyPressed.key.alt,
	                 ",control=", keyPressed.key.control, ",shift=", keyPressed.key.shift, ",system=", keyPressed.key.system, "}");
}

void InputEcho::receive(KeyReleased& keyReleased) {
	debugLogger.info("Released key '", (unsigned int)keyReleased.key.code, "'{alt=", keyReleased.key.alt,
	                 ",control=", keyReleased.key.control, ",shift=", keyReleased.key.shift, ",system=", keyReleased.key.system, "}");
}

void InputEcho::receive(TextEntered& textEntered) {
	debugLogger.info("Entered text{unicode='", (unsigned int)textEntered.text.unicode,
	                 "',asAscii='", (char)textEntered.text.unicode, "'}");
}

void InputEcho::receive(MouseButtonPressed& mouseButtonPressed) {
	debugLogger.info("Pressed mouse button{code='", (unsigned int)mouseButtonPressed.button.button,
	                 "',position=<", mouseButtonPressed.button.x, ", ", mouseButtonPressed.button.x, ">}");
}

void InputEcho::receive(MouseButtonReleased& mouseButtonReleased) {
	debugLogger.info("Released mouse button{code='", (unsigned int)mouseButtonReleased.button.button,
	                 "',position=<", mouseButtonReleased.button.x, ", ", mouseButtonReleased.button.x, ">}");
}

void InputEcho::receive(MouseMoved& mouseMoved) {
	debugLogger.info("Mouse location{", mouseMoved.where.x, ", ", mouseMoved.where.y, "}"
	                 "\nMouse delta{", mouseMoved.delta.x, ", ", mouseMoved.delta.y, "}",
	                 "\nWorld delta{", mouseMoved.worldDelta.x, ", ", mouseMoved.worldDelta.y, "}");
}

void InputEcho::receive(MouseWheelMoved& mouseWheelMoved) {
	debugLogger.info("Mouse Wheel Moved{ticks: ", mouseWheelMoved.ticks,
	                 ", mousePos{", mouseWheelMoved.x, ", ", mouseWheelMoved.y, "}}");
}
