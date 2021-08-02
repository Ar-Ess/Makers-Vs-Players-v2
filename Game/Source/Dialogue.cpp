#include "Dialogue.h"
#include "DialogueNode.h"
#include "DialogueOption.h"

void Dialogue::AssignOptions()
{
	for (ListItem<DialogueNode*>* nodeItem = this->nodeList.start;
		nodeItem != nullptr; nodeItem = nodeItem->next)
	{
		if (nodeItem->data->optionsList.Count() != 0)
		{
			for (ListItem<DialogueOption*>* optionItem = nodeItem->data->optionsList.start;
				optionItem != nullptr; optionItem = optionItem->next)
			{
				for (ListItem<DialogueNode*>* nodeItem2 = this->nodeList.start;
					nodeItem2 != nullptr; nodeItem2 = nodeItem2->next)
				{
					if (nodeItem2->data->nodeID == optionItem->data->nextNodeID)
					{
						optionItem->data->nextNode = nodeItem2->data;
					}
				}
			}
		}
		else continue;
	}
}
