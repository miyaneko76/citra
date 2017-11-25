﻿// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <QKeyEvent>
#include <QWidget>
#include <boost/optional.hpp>
#include "common/param_package.h"
#include "core/settings.h"
#include "input_common/main.h"
#include "ui_configure_input.h"

class QPushButton;
class QString;
class QTimer;

namespace Ui {
class ConfigureInput;
}

class ConfigureInput : public QWidget {
    Q_OBJECT

public:
    explicit ConfigureInput(QWidget* parent = nullptr);

    /// Save all button configurations to settings file
    void applyConfiguration();

private:
    std::unique_ptr<Ui::ConfigureInput> ui;

    std::unique_ptr<QTimer> timeout_timer;
    std::unique_ptr<QTimer> poll_timer;

    /// This will be the the setting function when an input is awaiting configuration.
    boost::optional<std::function<void(const Common::ParamPackage&)>> input_setter;

    std::array<Common::ParamPackage, Settings::NativeButton::NumButtons> buttons_param;
    std::array<Common::ParamPackage, Settings::NativeAnalog::NumAnalogs> analogs_param;

    static constexpr int ANALOG_SUB_BUTTONS_NUM = 5;

    /// Each button input is represented by a QPushButton.
    std::array<QPushButton*, Settings::NativeButton::NumButtons> button_map;

    /// A group of five QPushButtons represent one analog input. The buttons each represent up,
    /// down, left, right, and modifier, respectively.
    std::array<std::array<QPushButton*, ANALOG_SUB_BUTTONS_NUM>, Settings::NativeAnalog::NumAnalogs>
        analog_map_buttons;

    /// Analog inputs are also represented each with a single button, used to configure with an
    /// actual analog stick
    std::array<QPushButton*, Settings::NativeAnalog::NumAnalogs> analog_map_stick;

    static const std::array<std::string, ANALOG_SUB_BUTTONS_NUM> analog_sub_buttons;

    std::vector<std::unique_ptr<InputCommon::Polling::DevicePoller>> device_pollers;

    /// A flag to indicate if keyboard keys are okay when configuring an input. If this is false,
    /// keyboard events are ignored.
    bool want_keyboard_keys = false;

    /// Load configuration settings.
    void loadConfiguration();
    /// Restore all buttons to their default values.
    void restoreDefaults();
    /// Update UI to reflect current configuration.
    void updateButtonLabels();

    /// Called when the button was pressed.
    void handleClick(QPushButton* button,
                     std::function<void(const Common::ParamPackage&)> new_input_setter,
                     InputCommon::Polling::DeviceType type);

    /// Finish polling and configure input using the input_setter
    void setPollingResult(const Common::ParamPackage& params, bool abort);

    /// Handle key press events.
    void keyPressEvent(QKeyEvent* event) override;
};
