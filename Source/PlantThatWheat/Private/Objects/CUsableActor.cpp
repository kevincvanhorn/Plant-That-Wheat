#include "CUsableActor.h"
#include "CCharacterBase.h"

ACUsableActor::ACUsableActor() {
	bOutlineEnabled = false;
}

bool ACUsableActor::OnUsed_Implementation(ACMultiTool * Tool)
{
	return false;
}

bool ACUsableActor::StartFocus_Implementation()
{
	return false;
}

bool ACUsableActor::EndFocus_Implementation()
{
	return false;
}
