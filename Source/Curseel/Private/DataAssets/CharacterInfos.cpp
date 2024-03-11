#include "DataAssets/CharacterInfos.h"

FText UCharacterInfos::GetNameWithStyle() const {
    FString tmp = RichTextStyleName;
    tmp.InsertAt(RichTextStyleName.Len() - 3, CharacterName.ToString());
    FText text = text.FromString(tmp);
    return text;
}
