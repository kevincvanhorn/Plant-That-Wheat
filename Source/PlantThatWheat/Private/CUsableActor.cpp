#include "CUsableActor.h"

ACUsableActor::ACUsableActor() {

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
