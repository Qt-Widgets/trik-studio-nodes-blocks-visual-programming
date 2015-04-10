/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>

#include "customizer.h"
#include "generator.h"

namespace ubiq {
namespace generator {

/** Entry point for generator plugin, through this class QReal GUI loads and
	runs a plugin. It provides a list of actions that user can do with a plugin
	(basically there is only "generate" action), and GUI customization object.
*/
class UbiqGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "ubiq.UbiqGeneratorPlugin")

public:
	UbiqGeneratorPlugin();
	virtual ~UbiqGeneratorPlugin();

	/// Initializes generator with ref to logical model.
	virtual void init(qReal::PluginConfigurator const &configurator);

	/// Returns list containing "generate" action, which runs a generator.
	virtual QList<qReal::ActionInfo> actions();

	/// Provides customization object that changes main window title and so on.
	virtual qReal::Customizer* customizationInterface();

private:
	/// Customization object for configuring QReal main window.
	Customizer mCustomizer;

	/// Generator main object, directs generation.
	Generator mGenerator;
};

}
}
