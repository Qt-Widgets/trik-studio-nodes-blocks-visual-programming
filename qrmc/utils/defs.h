#pragma once
#include <QtCore/QString>

QString const endline = "\n";
QString const nodeIndent = "\t\t\t";

QString const generatedDir = "../qrmc/plugins";
QString const shapesDir = "shapes";
QString const generatedShapesDir = "generated";
QString const pluginHeaderName = "pluginInterface.h";
QString const pluginSourceName = "pluginInterface.cpp";
QString const elementsFileName = "elements.h";
QString const resourceFileName = "plugin.qrc";
QString const pluginsProjectFileName = "plugin.pro";

QString const templatesDir = "templates";
QString const pluginHeaderTemplate = "pluginInterface.h.template";
QString const pluginSourceTemplate = "pluginInterface.cpp.template";
QString const elementsHeaderTemplate = "elements.h.template";
QString const nodeClassTemplate = "node.template";
QString const edgeClassTemplate = "edge.template";
QString const utilsTemplate = "utils.template";
QString const resourceTemplate = "plugin.qrc.template";
QString const projectTemplate = "pro.template";
QString const pluginsProjectTemplate = "plugins.pro.template";

QString const destinationDir = "@@Destination@@";
QString const pathToSources = "@@PathToSources@@";
QString const subdirsTag = "@@Subdirs@@";
QString const metamodelNameTag = "@@MetamodelName@@";
QString const initDiagramNameMapLineTag = "@@InitDiagramNameMapLine@@";
QString const initDiagramNodeNameMapLineTag = "@@InitDiagramNodeNameMapLine@@";
QString const initPropertyDisplayedNamesTag = "@@InitPropertyNamesMapLine@@";
QString const initElementNameMapLineTag = "@@InitElementNameMapLine@@";
QString const initParentsMapLineTag = "@@InitParentsMapLine@@";
QString const initPropertyTypesMapLineTag = "@@InitPropertyTypesMapLine@@";
QString const initMouseGesturesMapLineTag = "@@InitMouseGesturesMapLine@@";
QString const getGraphicalObjectLineTag = "@@GetGraphicalObjectLine@@";
QString const initPropertyDefaultMapLineTag = "@@InitPropertyDefaultMapLine@@";
QString const getContainersLineTag = "@@GetContainedByLine@@";
QString const getReferencePropertiesLineTag = "@@GetReferencePropertiesLine@@";
QString const getConnectionsLineTag = "@@GetConnectedTypesLine@@";
QString const getUsagesLineTag = "@@GetUsedTypesLine@@";
QString const getIsNodeOrEdgeLineTag = "@@IsNodeOrEdgeLine@@";
QString const getEnumsLineTag = "@@GetEnumValuesLine@@";
QString const getPossibleEdgesLineTag = "@@GetPossibleEdgesLine@@";
QString const sdfPortsTag = "@@PortsSdf@@";
QString const linePortTag = "@@LinePort@@";
QString const pointPortTag = "@@PointPort@@";
QString const portsTag = "@@Ports@@";
QString const nodeUnusedTag = "@@NodeUnusedArgs@@";
QString const nodeLoadShapeRendererTag = "@@NodeLoadShapeRenderer@@";
QString const nodeLoadPortsRendererTag = "@@NodeLoadPortsRenderer@@";
QString const nodeContentsTag = "@@NodeSetContents@@";
QString const nodePointPortInitTag = "@@NodeInitPointPort@@";
QString const nodeLinePortInitTag = "@@NodeInitLinePort@@";
QString const nodeInitPortsTag = "@@NodeInitPorts@@";
QString const nodeInitTag = "@@NodeInit@@";
QString const edgeInitTag = "@@EdgeInit@@";
QString const updateDataTag = "@@UpdateData@@";
QString const nodeBorderTag = "@@NodeBorder@@";
QString const nodeValidBorderTag = "@@NodeValidBorder@@";
QString const nodeInvalidBorderTag = "@@NodeInvalidBorder@@";
QString const labelDefinitionTag = "@@LabelDefinition@@";
QString const nodeConstructorTag = "@@NodeConstructor@@";
QString const contextMenuItems = "@@ContextMenuItems@@";
QString const itemsList = "@@ItemsList@@";
QString const itemsValidList = "@@ItemsValidList@@";
QString const itemsInvalidList = "@@ItemsInvalidList@@";

QString const diagramNameTag = "@@DiagramName@@";
QString const diagramNodeNameTag = "@@DiagramNodeName@@";
QString const diagramDisplayedNameTag = "@@DiagramDisplayedName@@";
QString const elementNameTag = "@@ElementName@@";
QString const elementDisplayedNameTag = "@@ElementDisplayedName@@";
QString const propertyDisplayedNameTag = "@@PropertyDisplayedName@@";
QString const gesturePathTag = "@@GesturePath@@";
QString const propertyNameTag = "@@PropertyName@@";
QString const propertyTypeTag = "@@PropertyType@@";
QString const childNameTag = "@@ChildElementName@@";
QString const parentNameTag = "@@ParentElementName@@";
QString const propertyDefaultTag = "@@PropertyDefaultValue@@";
QString const containersListTag = "@@ContainedByList@@";
QString const connectionsListTag = "@@ConnectedToList@@";
QString const referencePropertiesListTag = "@@ReferencePropertiesList@@";
QString const usagesListTag = "@@UsedTypesList@@";
QString const isNodeOrEdgeTag = "@@IsNodeOrEdgeValue@@";
QString const enumsListTag = "@@EnumValuesList@@";
QString const possibleEdgesListTag = "@@PossibleEdgesList@@";
QString const startXTag = "@@StartX@@";
QString const startYTag = "@@StartY@@";
QString const endXTag = "@@EndX@@";
QString const endYTag = "@@EndY@@";
QString const pointXTag = "@@PointX@@";
QString const pointYTag = "@@PointY@@";
QString const nodeWidthTag = "@@NodeWidth@@";
QString const nodeHeightTag = "@@NodeHeight@@";
QString const startXScalabilityTag = "@@NodeLinePortStartXScalable@@";
QString const startYScalabilityTag = "@@NodeLinePortStartYScalable@@";
QString const endXScalabilityTag = "@@NodeLinePortEndXScalable@@";
QString const endYScalabilityTag = "@@NodeLinePortEndYScalable@@";
QString const labelXTag = "@@LabelX@@";
QString const labelYTag = "@@LabelY@@";
QString const labelNameTag = "@@LabelName@@";
QString const labelReadonlyTag = "@@LabelReadonly@@";
QString const labelIndexTag = "@@Count@@";
QString const nameRoleTag ="@@NameRole@@";
QString const customRoleTag ="@@CustomRole@@";
QString const updateRoleTag ="@@UpdateRole@@";
QString const hasPortsTag = "@@NodeHasPorts@@";
QString const isNodeTag = "@@IsNode@@";
QString const isContainerTag = "@@IsContainer@@";
QString const sortingContainerTag = "@@IsSortContainer@@";
QString const minimizeToChildrenTag = "@@MinimizesToChildren@@";
QString const maximizeChildrenTag = "@@MaximizesChildren@@";
QString const hasMovableChildrenTag = "@@HasMovableChildren@@";
QString const forestallingSizeTag = "@@SizeOfForestalling@@";
QString const childrenForestallingSizeTag = "@@SizeOfChildrenForestalling@@";
QString const isPortTag = "@@IsPort@@";
QString const hasPinTag = "@@HasPin@@";
QString const lineSdfTag = "@@LineSdf@@";
QString const lineTypeTag = "@@LineType@@";
QString const isResizeable = "@@IsResizeable@@";
QString const xCoordIsScalable = "@@XCoordIsScalable@@";
QString const yCoordIsScalable = "@@YCoordIsScalable@@";

QString const nodesListTag = "@@NodesList@@";
QString const edgesListTag = "@@EdgesList@@";
QString const sdfFileTag = "@@SdfFile@@";
QString const fileNameTag = "@@Filename@@";

QString const arrowTemplateTag = "@@Arrow@@";
QString const openArrowTemplateTag = "@@OpenArrow@@";
QString const rhombTemplateTag = "@@Rhomb@@";
QString const complexArrowTemplateTag = "@@ComplexArrow@@";
QString const filledArrowColorTag = "@@FilledArrowColor@@";
QString const emptyArrowColorTag = "@@EmptyArrowColor@@";
QString const beginArrowBrushColorTag = "@@BeginArrowBrushColor@@";
QString const endArrowBrushColorTag = "@@EndArrowBrushColor@@";
QString const beginArrowCustomizationTag = "@@BeginArrowCustomization@@";
QString const endArrowCustomizationTag = "@@EndArrowCustomization@@";

QString const metamodelDiagram = "MetamodelDiagram";
QString const metaEditorDiagramNode = "MetaEditorDiagramNode";
QString const metaEntityNode = "MetaEntityNode";
QString const metaEntityEdge ="MetaEntityEdge";
QString const metaEntityImport = "MetaEntityImport";
QString const metaEntityEnum = "MetaEntityEnum";
QString const metaEntityValue = "MetaEntityValue";
QString const metaEntityPossibleEdge = "MetaEntityPossibleEdge";
QString const metaEntityPropertiesAsContainer = "MetaEntityPropertiesAsContainer";
QString const metaEntityParent = "MetaEntityParent";
QString const metaEntityAttribute = "MetaEntity_Attribute";
QString const metaEntityConnection = "MetaEntityConnection";
QString const metaEntityUsage = "MetaEntityUsage";
QString const metaEntityContextMenuField = "MetaEntityContextMenuField";
QString const metaEntityAssociation = "MetaEntityAssociation";

QString const nameOfTheDirectory = "name of the directory";


