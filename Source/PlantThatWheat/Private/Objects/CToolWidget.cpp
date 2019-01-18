

#include "CToolWidget.h"

void UCToolWidget::SwitchTool(uint8 NewToolIndex) {
	CurToolIndex = NewToolIndex;
	//UE_LOG(LogTemp, Warning, TEXT("SWITCH MODE %f"), OffsetY);
	this->OnSwitchTool((CurToolIndex) / (float)(NumTools+2));
}	

void UCToolWidget::Init(int32 NumTools, uint8 CurToolIndex) {
	this->NumTools = NumTools;
	this->CurToolIndex = CurToolIndex;
}