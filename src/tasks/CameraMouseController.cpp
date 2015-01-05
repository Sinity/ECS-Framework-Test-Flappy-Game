#include <SFML/Graphics/RenderWindow.hpp>
#include "CameraMouseController.h"
#include "../events/system/MouseWheelMoved.h"
#include "../events/system/MouseButtonPressed.h"
#include "../events/system/MouseButtonReleased.h"
#include "../events/system/MouseMoved.h"

void CameraMouseController::update() {
}

void CameraMouseController::receive(MouseWheelMoved& mouseWheelEvent) {
	auto currentView = window.getView();
	currentView.zoom(1.f + zoomFactor * mouseWheelEvent.ticks);
	window.setView(currentView);
}

void CameraMouseController::receive(MouseButtonPressed& mousePress) {
	if(mousePress.button.button == panViewMouseButtonCode) {
		mouseMode = PanView;
	}
}

void CameraMouseController::receive(MouseButtonReleased& mouseRelease) {
	if(mouseRelease.button.button == panViewMouseButtonCode) {
		mouseMode = Nothing;
	}
}

void CameraMouseController::receive(MouseMoved& mouseMoved) {
	if(mouseMode == PanView) {
		auto currentView = window.getView();
		currentView.move(-mouseMoved.worldDelta.x * panViewXFactor, -mouseMoved.worldDelta.y * panViewYFactor);
		window.setView(currentView);
	}
}

CameraMouseController::CameraMouseController(Engine& engine, sf::RenderWindow& window) :
		Task(engine),
		window(window),
		zoomFactor(engine.config.get("gameplay.camera.zoom.factor", 0.1f)),
		panViewXFactor(engine.config.get("gameplay.camera.pan.factor.x", 1.f)),
		panViewYFactor(engine.config.get("gameplay.camera.pan.factor.y", 1.f)),
		panViewMouseButtonCode(engine.config.get("gameplay.camera.pan.mouseButtonCode", 2u)) {
	engine.events.connect<MouseWheelMoved>(*this);
	engine.events.connect<MouseButtonPressed>(*this);
	engine.events.connect<MouseButtonReleased>(*this);
	engine.events.connect<MouseMoved>(*this);
}
