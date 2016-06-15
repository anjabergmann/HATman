.lib "libGladiator.dll"
ConnectToMarauroaServer$(server$,port%,username$,password$):"_ConnectToMarauroaServer@16"

AvailableCharacters%():"_AvailableCharacters@0"
ResetCharacter():"_ResetCharacter@0"
HasNextCharacter%():"_HasNextCharacter@0"
NextCharacter$():"_NextCharacter@0"

ChooseCharacter$(character$):"_ChooseCharacter@4"

HasNewPerception%():"_HasNewPerception@0"
IsSyncedWithServer%():"_IsSyncedWithServer@0"
DisconnectFromMarauroaServer$():"_DisconnectFromMarauroaServer@0"

ArenaGet$(attribute$):"_ArenaGet@4"
ArenaPut(attribute$,value$):"_ArenaPut@8"
ArenaFirstGladiator%():"_ArenaFirstGladiator@0"

ArenaResetGladiator():"_ArenaResetGladiator@0"
ArenaHasNextGladiator%():"_ArenaHasNextGladiator@0"
ArenaNextGladiator%():"_ArenaNextGladiator@0"
ArenaFindGladiator%(id%):"_ArenaFindGladiator@4"

GladiatorGet$(attribute$):"_GladiatorGet@4"
GladiatorHas%(attribute$):"_GladiatorHas@4"
GladiatorPut(attribute$, value$):"_GladiatorPut@8"

Player%():"_Player@0"
NumPlayers%():"_NumPlayers@0"
ResetPlayer():"_ResetPlayer@0"
HasNextPlayer%():"_HasNextPlayer@0"
NextPlayer():"_NextPlayer@0"
FindPlayer%(id%):"_FindPlayer@4"

PlayerGet$(attribute$):"_PlayerGet@4"
PlayerHas%(attribute$):"_PlayerHas@4"
PlayerPut(attribute$, value$):"_PlayerPut@8"

ActionRequestFight%(wait_reply%):"_actionRequestFight@4"
ActionFightMode%(wait_reply%, mode$):"_actionFightMode@8"
ActionVote%(wait_reply%, vote$):"_actionVote@8"
ActionChat%(wait_reply%, text$):"_actionChat@8"

trace(event$, text$):"_trace@8"