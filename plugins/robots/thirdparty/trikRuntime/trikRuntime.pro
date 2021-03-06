# Copyright 2016 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TEMPLATE = subdirs

include(../../../../global.pri)

SUBDIRS += \
	PythonQt \
	trikKernel \
	trikNetwork \
	trikControl \
	trikHal \
	trikScriptRunner \

# qslog.file = $$PWD/../../../../thirdparty/qslog/QsLogSharedLibrary.pro
# qslog.subdir = $$PWD/../../../../thirdparty/qslog/

PythonQt.subdir = $$PWD/trikRuntime/PythonQt
trikScriptRunner.subdir = $$PWD/trikRuntime/trikScriptRunner
trikKernel.subdir = $$PWD/trikRuntime/trikKernel
trikNetwork.subdir = $$PWD/trikRuntime/trikNetwork
trikControl.subdir = $$PWD/trikRuntime/trikControl
trikHal.subdir = $$PWD/trikRuntime/trikHal
trikControl.depends = trikKernel trikHal
trikKernel.depends =
trikNetwork.depends = trikKernel
trikScriptRunner.depends = trikControl trikKernel trikNetwork PythonQt
trikHal.depends = trikKernel
PythonQt.depends =

