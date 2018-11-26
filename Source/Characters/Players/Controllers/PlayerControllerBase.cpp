// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "../PlayerCharacterBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../Enemies/EnemyCharacterBase.h"
#include "Characters/Players/Components/CombatComponent.h"
#include "Characters/Players/Components/EquipmentComponent.h"
#include "GameStates/DungeonGameState.h"
#include "../Components/InventoryComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "GamePlayerState.h"
#include "LevelControlers/PersistentLevelControler.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../UI/PlayerAbilityBar.h"
#include "LevelActors/DungeonTreasureChest.h"


APlayerControllerBase::APlayerControllerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComp = ObjectInitializer.CreateDefaultSubobject<UInventoryComponent>(this, FName("Inventory Component"));
	AbilityComp = ObjectInitializer.CreateDefaultSubobject<UAbilityManagerComponent>(this, FName("Ability Component"));

}


void APlayerControllerBase::BeginPlayingState()
{
	Super::BeginPlayingState();
	
}

void APlayerControllerBase::EndPlayingState()
{
	Super::EndPlayingState();

}

void APlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

    if (HasTargetLocked && CurrentTarget.IsValid() && PlayerCharacter.IsValid()) {
        
		if (CurrentTarget->GetIsEnemyAlive())
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector TargetLocation = CurrentTarget->GetActorLocation();
			FVector Direction = TargetLocation - PlayerLocation;
			Direction.Normalize();

			FRotator rotation = Direction.ToOrientationRotator();

			SetControlRotation(rotation);
			PlayerCharacter->SetActorRotation(rotation);
		}
		else
		{
			TargetLock();
			CurrentTarget = nullptr;
		}
    }
    else if (HasTargetLocked && !CurrentTarget.IsValid()) {
        TargetLock();
    }

	UpdatePlayerUI();
}

void APlayerControllerBase::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);
	PlayerCharacter = Cast<APlayerCharacterBase>(aPawn);

	if (PlayerCharacter.IsValid())
	{
		SetupInputComponent();

		if (IsValid(PlayerUIClass) && !PlayerUI.IsValid())
		{
			ABattleCharacterBase* BattleCharacter = PlayerCharacter.Get();
			

			PlayerUI = CreateWidget<UPlayerWidgetHUD>(this, PlayerUIClass);

			if (PlayerUI.IsValid()) {
				PlayerUI->AddToPlayerScreen();
				UpdatePlayerUI();
				//populate abilities into playerui
				//UI needs functions to update the ability bar
				//make sure to remove click attack from player controller, replace click to click on enemies
				//maybe click on door for interaction instead of bumping into it
				if (IsValid(AbilityComp))
				{
					UPlayerAbilityBar* AbilityBar = PlayerUI->GetAbilityBar();
					if (IsValid(AbilityBar))
					{
						AbilityBar->RegisterAbilityManager(AbilityComp, true);
					}


					bShowMouseCursor = true;
				}
			}

		}
	}
	
}

void APlayerControllerBase::UnPossess()
{
	Super::UnPossess();
	PlayerCharacter = nullptr;
	if (PlayerUI.IsValid()) {
		PlayerUI->RemoveFromParent();
	}
}

bool APlayerControllerBase::AddLootableItemToInventory(const FName& ItemName, const TSubclassOf<UItemObject> LootedItemClass)
{
	bool success = InventoryComp->AddLootableItemToInventory(ItemName, LootedItemClass);

	if (bIsShowingInventory && bIsShowingStatsUI)
	{
		PlayerUI->ShowInventoryPanel(bIsShowingInventory, InventoryComp);
		PlayerUI->ShowHideStatsUI(bIsShowingStatsUI, PlayerCharacter->GetCombatControler());
	}

	return success;
}


bool APlayerControllerBase::UpdateInventoryItemButtonIndex(const FName& KeyName, const uint8 index)
{
	if (IsValid(InventoryComp))
	{
		return InventoryComp->UpdateItemButtonIndex(KeyName, index);
	}

	return false;
}

void APlayerControllerBase::TakeSelectedLoot(const TArray<FName>& KeyNames)
{
	if (CurrentInteractChest.IsValid())
	{
		const TMap<FName, FLootItemData > &Items = CurrentInteractChest->GetLootItems();

		for (int index = 0; index < KeyNames.Num(); index++)
		{
			const FName& Key = KeyNames[index];
			const FLootItemData* ItemData = Items.Find(Key);

			if (ItemData)
			{
				AddLootableItemToInventory(Key, ItemData->LootItemClass);
			}
		}
	}
}

void APlayerControllerBase::ExitInteratingMode()
{
	bIsInteracting = false;
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

bool APlayerControllerBase::UseInventoryItem(const FInventoryItemInfo& ItemInfo)
{
	bool success = InventoryComp->UseItem(ItemInfo.InventoryKeyName, ItemInfo.Type);

	if (success)
	{
		switch (ItemInfo.Type)
		{
		case ItemTypes::ITEM_NONE:
			break;
		case ItemTypes::ITEM_DOOR_KEY:
			break;
		case ItemTypes::ITEM_EQUIPABLE_WEAPON:
			break;
		case ItemTypes::ITEM_EQUIPABLE_ARMOR:
			break;
		case ItemTypes::ITEM_CONSUMABLE:
		{

			break;
		}
		case ItemTypes::ITEM_CURRENCY:
			break;
		case ItemTypes::ITEM_TOOL:
			break;
		default:
			break;
		}
	}
	return success;
}

void APlayerControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABattleCharacterBase* player = PlayerCharacter.Get();
	
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (PlayerCharacter.IsValid())
	{
		InputComponent->BindAxis(FName("Forward"), this, &APlayerControllerBase::ForwardMovement);
		InputComponent->BindAxis(FName("Right"), this, &APlayerControllerBase::RightMovement);
		InputComponent->BindAxis(FName("MouseX"), this, &APlayerControllerBase::MouseX);
		InputComponent->BindAxis(FName("MouseY"), this, &APlayerControllerBase::MouseY);

		InputComponent->BindAction(FName("TargetNearestForward"), EInputEvent::IE_Released, this, &APlayerControllerBase::TargetNearest);
		InputComponent->BindAction(FName("TargetLastHit"), EInputEvent::IE_Released, this, &APlayerControllerBase::TargetLastHit);
		InputComponent->BindAction(FName("TargetLock"), EInputEvent::IE_Released, this, &APlayerControllerBase::TargetLock);
		InputComponent->BindAction(FName("SelectObject"), EInputEvent::IE_Released, this, &APlayerControllerBase::ClickOnObject);
		InputComponent->BindAction(FName("CharacterStats"), EInputEvent::IE_Released, this, &APlayerControllerBase::ShowHideCharacterStats);
		InputComponent->BindAction(FName("ToggleCombatMode"), EInputEvent::IE_Released, this, &APlayerControllerBase::ToggleCombatMode);
		InputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &APlayerControllerBase::StartActionJump);
		InputComponent->BindAction(FName("Jump"), EInputEvent::IE_Released, this, &APlayerControllerBase::StopActionJump);
		InputComponent->BindAction(FName("Inventory"), EInputEvent::IE_Released, this, &APlayerControllerBase::DisplayItemInventory);

	}
}

void APlayerControllerBase::UpdatePlayerUI()
{
	if (PlayerUI.IsValid() && PlayerCharacter.IsValid()) {
		const UCombatData* data = PlayerCharacter->GetCharacterCombatData();
		if (IsValid(data)) {
			PlayerUI->UpdatePlayerHealthPanel(data);
			PlayerUI->UpdateExpBar();
			
		}
	}
	
}

void APlayerControllerBase::ForwardMovement(float value)
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		if (PlayerCharacter.IsValid() && value != 0.0f)
		{
			PlayerCharacter->PerformForwardMovement(value);
		}
	}
}

void APlayerControllerBase::RightMovement(float value)
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		if (PlayerCharacter.IsValid() && value != 0.0f)
		{
			PlayerCharacter->PerformRightMovement(value);

		}
	}
}

void APlayerControllerBase::MouseX(float value)
{
	FRotator rotation = GetControlRotation();

    if (!HasTargetLocked) {
        
        rotation.Yaw += value;
		
		SetControlRotation(rotation);		

    }
    else if (PlayerCharacter.IsValid()) {
 
        PlayerCharacter->SetSpringArmRotationX(value);

    }
	
}

void APlayerControllerBase::MouseY(float value)
{
    if (PlayerCharacter.IsValid())
    {
        PlayerCharacter->MouseY(value);
    }
}

void APlayerControllerBase::TargetNearest()
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		//GetWorld()->bDebugDrawAllTraceTags = true;

		if (PlayerCharacter.IsValid())
		{
			TArray<AActor*> FoundEnemies;
			TArray<TWeakObjectPtr<AEnemyCharacterBase>> NearByEnemies;

			UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacterBase::StaticClass(), FoundEnemies);

			for (int index = 0; index < FoundEnemies.Num(); ++index) {
				TWeakObjectPtr<AEnemyCharacterBase> target = Cast<AEnemyCharacterBase>(FoundEnemies[index]);
				if (target.IsValid() && target->IsValidLowLevelFast() && target->GetIsEnemyAlive()) {
					FVector targetLocation = target->GetActorLocation();
					FVector location = PlayerCharacter->GetActorLocation();

					location.Z += 150.0f;
					FVector direction = targetLocation - location;
					float distance;

					direction.ToDirectionAndLength(direction, distance);
					FHitResult Hit;
					FCollisionQueryParams QueryParams;
					QueryParams.AddIgnoredActor(PlayerCharacter.Get());
					if (GetWorld()->LineTraceSingleByProfile(Hit, location, targetLocation, FName("Pawn"), QueryParams)) {
						if (Hit.Actor.IsValid() && Hit.Actor == target) {
							if (distance < NearestTargetDistanceMax) {
								NearByEnemies.Add(target);
							}
						}
					}
				}
			}

			if (NearByEnemies.Num() > 0) {
				float near = NearestTargetDistanceMax;
				int near_index = 0;

				for (int index = 0; index < NearByEnemies.Num(); ++index) {
					FVector targetLocation = NearByEnemies[index]->GetActorLocation();
					FVector location = PlayerCharacter->GetActorLocation();

					FVector direction = targetLocation - location;
					float distance;

					direction.ToDirectionAndLength(direction, distance);

					if (near > distance) {
						near = distance;
						near_index = index;
					}
				}
				if (CurrentTarget.IsValid())
				{
					CurrentTarget->SetEnableTargetLock(false);
				}
				
				CurrentTarget = NearByEnemies[near_index];
				CurrentTarget->SetHighlightedAsTarget(true);

				if (CurrentTarget.IsValid())
				{
					CurrentTarget->SetHighlightedAsTarget(true);
					PlayerUI->ShowHideEnemyHealthPanel(CurrentTarget->GetCharacterCombatData(), ESlateVisibility::Visible);
				}
				else
				{
					PlayerUI->ShowHideEnemyHealthPanel(nullptr, ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void APlayerControllerBase::TargetLastHit()
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility && bIsInCombatMode)
	{
		if (PlayerCharacter.IsValid()) {

			CurrentTarget = LastHitTarget;

		}
	}
}

void APlayerControllerBase::TargetLock()
{
	if (bIsInCombatMode && !bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		HasTargetLocked = !HasTargetLocked;

		if (PlayerCharacter.IsValid()) {

			PlayerCharacter->SetHasTargetLocked(HasTargetLocked);
			if (CurrentTarget.IsValid())
			{
				CurrentTarget->SetEnableTargetLock(HasTargetLocked);
			}
		}
	}
}

void APlayerControllerBase::StartActionJump()
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		if (PlayerCharacter.IsValid())
		{
			PlayerCharacter->StartActionJump();
		}
	}
}

void APlayerControllerBase::StopActionJump()
{
	if (!bIsInteracting && !bIsTogglingCombat)
	{
		if (PlayerCharacter.IsValid())
		{
			PlayerCharacter->StopActionJump();
		}
	}
}

void APlayerControllerBase::NotifyAttackComplete()
{
	bIsUsingAbility = false;
	PlayerUI->NotifyAbilityComplete();
}


void APlayerControllerBase::ShowHideStatsUI()
{
	bIsShowingStatsUI = !bIsShowingStatsUI;

	PlayerUI->ShowHideStatsUI(bIsShowingStatsUI, PlayerCharacter->GetCombatControler());
}


AEnemyCharacterBase* APlayerControllerBase::GetCurrentTarget()
{
	if (CurrentTarget.IsValid())
	{
		return CurrentTarget.Get();
	}

	return nullptr;
}

const UAttackAbility* APlayerControllerBase::GetSelectedAbility() const
{
	if (SelectedAttackAbility.IsValid())
	{
		return SelectedAttackAbility.Get();
	}
	else if (SelectedRageAbility.Get())
	{
		return SelectedRageAbility.Get();

	}

	return nullptr;
}

const UCombatData* APlayerControllerBase::GetCharacterCombatData() const
{
	if (PlayerCharacter.IsValid())
	{
		return PlayerCharacter->GetCharacterCombatData();
	}

	return nullptr;
}

void APlayerControllerBase::UpdateEnemyHealthPanel()
{
	if (CurrentTarget.IsValid())
	{
		CurrentTarget->SetHighlightedAsTarget(true);
		PlayerUI->ShowHideEnemyHealthPanel(CurrentTarget->GetCharacterCombatData(), ESlateVisibility::Visible);
	}
	else
	{
		PlayerUI->ShowHideEnemyHealthPanel(nullptr, ESlateVisibility::Hidden);
	}
}


bool APlayerControllerBase::UnEquipItem(const FInventoryItemInfo* UnequipInfo)
{
	if (PlayerCharacter.IsValid())
	{
		return PlayerCharacter->UnEquipItem(UnequipInfo);
	}

	return false;
}

bool APlayerControllerBase::EquipItem(const FInventoryItemInfo* UnequipInfo)
{
	if (PlayerCharacter.IsValid())
	{
		return PlayerCharacter->EquipItem(UnequipInfo);
	}

	return false;
}

void APlayerControllerBase::ToggleCombatMode()
{
	if (!bIsInteracting && !bIsTogglingCombat && !bIsUsingAbility)
	{
		if (PlayerCharacter.IsValid())
		{
			PlayerCharacter->ToggleCombatMode();
			bIsTogglingCombat = true;
			bIsInCombatMode = !bIsInCombatMode;
		}
	}
}


void APlayerControllerBase::ClickOnObject()
{
	if (!bIsInteracting)
	{
		FHitResult Result;
		if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Result))
		{
			if (CurrentTarget.IsValid())
			{
				CurrentTarget->SetHighlightedAsTarget(false);
			}
			if (CurrentInteractDoor.IsValid())
			{
				CurrentInteractDoor->SetEnableHighlight(false);
			}
			if (CurrentInteractChest.IsValid())
			{
				CurrentInteractChest->SetEnableChestHighlight(false);
			}
			if (CurrentInteractChest.IsValid())
			{
				const TMap<FName, TSubclassOf<UItemObject>> Loot;

				PlayerUI->ShowHideChestLootWindow(Loot, false);
			}
			AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(Result.Actor);
			if (IsValid(Enemy))
			{
				CurrentTarget = Enemy;
				CurrentTarget->SetHighlightedAsTarget(true);
				PlayerUI->ShowHideEnemyHealthPanel(CurrentTarget->GetCharacterCombatData(), ESlateVisibility::Visible);
			}
			else
			{
				PlayerUI->ShowHideEnemyHealthPanel(nullptr, ESlateVisibility::Hidden);
			}

			ADungeonDoorActor* Door = Cast<ADungeonDoorActor>(Result.Actor);

			if (IsValid(Door))
			{
				CurrentInteractDoor = &(*Door);
				CurrentInteractDoor->SetEnableHighlight(true);

				const float Distance = FVector::Distance(PlayerCharacter->GetActorLocation(), CurrentInteractDoor->GetActorLocation());

				if (Distance < 300.0f)
				{
					ShowHideDoorInteractUI(Door);

				}
			}

			ADungeonTreasureChest* Chest = Cast<ADungeonTreasureChest>(Result.Actor);

			if (IsValid(Chest))
			{
				CurrentInteractChest = &(*Chest);
				CurrentInteractChest->SetEnableChestHighlight(true);
				const float Distance = FVector::Distance(PlayerCharacter->GetActorLocation(), Chest->GetActorLocation());

				if (Distance < 300.0f)
				{
					TMap<FName, TSubclassOf<UItemObject>> LootItems;
					CurrentInteractChest->GetRandomLootItems(LootItems);
					CurrentInteractChest->SetEnableChestOpen(true);
					PlayerUI->ShowHideChestLootWindow(LootItems, true);

					bIsInteracting = true;
				}
				else
				{
					const TMap<FName, TSubclassOf<UItemObject>> Loot;
					
					PlayerUI->ShowHideChestLootWindow(Loot, false);
				}

			}
		}
	}
}

void APlayerControllerBase::ShowHideCharacterStats()
{
	if (PlayerUI.IsValid() && !bIsInteracting)
	{
		bIsShowingStatsUI = !bIsShowingStatsUI;

		PlayerUI->ShowHideStatsUI(bIsShowingStatsUI, PlayerCharacter->GetCombatControler());

	}
}

void APlayerControllerBase::ShowHideDoorInteractUI(const ADungeonDoorActor* TheDoor)
{
	//search inventory for keys

	if (IsValid(TheDoor) && !bIsTogglingCombat && !bIsInCombatMode && !bIsInteracting)
	{
		CurrentInteractDoor = (ADungeonDoorActor*)TheDoor;
		bIsInteracting = true;
		bShowMouseCursor = true;
		
		bool CanOpen = true;
		bool CanUnlock = false;

		DoorKeyTypes KeyType = TheDoor->GetKeyTypeToOpenDoor();

		if (KeyType != DoorKeyTypes::NONE_KEY)
		{
			CanOpen = false;

			const TArray<FName>& ItemNames = InventoryComp->GetItemKeyNames();

			for (int index = 0; index < ItemNames.Num(); index++)
			{
				const FName& Key = ItemNames[index];
				const UItemObject* ItemObject = InventoryComp->GetItemByName(Key);

				if (IsValid(ItemObject))
				{
					UDungeonDoorKey* SelectedDungeonKey = (UDungeonDoorKey*)Cast<UDungeonDoorKey>(ItemObject);

					if (IsValid(SelectedDungeonKey))
					{
						if (KeyType == SelectedDungeonKey->GetKeyType() && SelectedDungeonKey->GetIsUseable())
						{
							//SelectedDungeonKeyName = Key;
							CanUnlock = true;
							CanOpen = true;
							break;
						}
						else
						{
							//SelectedDungeonKeyName = NAME_None;
						}
					}
				}
			}
		}
		else
		{
			CanUnlock = false;
			CanOpen = true;
			CurrentInteractDoor->SetDoorCanOpen(true);
		}

		if (PlayerUI.IsValid())
		{
			
			PlayerUI->ShowDoorInteractWindow(CanOpen, CanUnlock,
				FText::FromString(TheDoor->GetDoorDetails()),
				FText::FromName(TheDoor->GetDoorName()));
			
		}
		
	}
}

void APlayerControllerBase::UnlockSelectedDoor()
{
	if (CurrentInteractDoor.IsValid())
	{
		CurrentInteractDoor->SetDoorCanOpen(true);
	}
}

void APlayerControllerBase::OpenSelectedDoor()
{
	if (CurrentInteractDoor.IsValid() && CurrentInteractDoor->CanDoorBeOpened())
	{
		CurrentInteractDoor->PerformDoorOpenClose(true);
	}
}

void APlayerControllerBase::SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams /*= FViewTargetTransitionParams()*/)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);
}


void APlayerControllerBase::OnNotifyToggleCombatCompleted()
{
	bIsTogglingCombat = false;
}

void APlayerControllerBase::CancelDoorInteractUI()
{
	if (CurrentInteractDoor.IsValid() && !bIsTogglingCombat && !bIsInCombatMode && bIsInteracting)
	{
		
		bIsInteracting = false;

		PlayerUI->ShowDoorInteractWindow(false, false,
			FText::FromString(CurrentInteractDoor->GetDoorDetails()),
			FText::FromName(CurrentInteractDoor->GetDoorName()));
	}

	CurrentInteractDoor = nullptr;
}

bool APlayerControllerBase::ActivateAbility(const FAbilityInfo* Ability)
{
	if (Ability && bIsInCombatMode && !bIsUsingAbility)
	{
		const UAttackAbility* AttackAbility = Cast<UAttackAbility>(AbilityComp->GetGenericAbility(Ability->KeyName));

		const URageAttackAbility* RageAbility = Cast<URageAttackAbility>(AttackAbility);

		const UCombatData* CombatData = PlayerCharacter->GetCharacterCombatData();

		if (IsValid(RageAbility))
		{
			if (AbilityComp->UseAbility(Ability->KeyName, &CombatData))
			{
				if (RageAbility->bIsAreaAttack)
				{
					PlayerCharacter->PerformAreaAttack(RageAbility->AnimationSectionName, false);
				}
				else
				{
					PlayerCharacter->PerformRageAttack(RageAbility->AnimationSectionName);
				}
				
				SelectedRageAbility = (URageAttackAbility*)RageAbility;
				SelectedAttackAbility = nullptr;
				bIsUsingAbility = true;
			}
		}
		else
		{
			if (AbilityComp->UseAbility(Ability->KeyName, &CombatData))
			{
				bUsingComboAbility = false;

				if (AttackAbility->bHasComboAttack && ComboKeyName == Ability->AbilityName)
				{
					bUsingComboAbility = true;
				}
				else if (AttackAbility->bHasComboAttack)
				{
					ComboKeyName = AttackAbility->ComboWithAbilityName;
				}
				else
				{
					ComboKeyName = NAME_None;
				}

				if (AttackAbility->bIsAreaAttack)
				{
					PlayerCharacter->PerformAreaAttack(AttackAbility->AnimationSectionName, false);
				}
				else
				{
					PlayerCharacter->PerformSingleAttack(AttackAbility->AnimationSectionName);
				}
				
				SelectedAttackAbility = (UAttackAbility*)AttackAbility;
				SelectedRageAbility = nullptr;
				bIsUsingAbility = true;
			}
		}
	}

	return bIsUsingAbility;
}


void APlayerControllerBase::DisplayItemInventory()
{
	TWeakObjectPtr<AGamePlayerState> GamePlayerState = Cast<AGamePlayerState>(PlayerState);
	if (GamePlayerState.IsValid())
	{
		bIsShowingInventory = !bIsShowingInventory;

		if (bIsShowingInventory)
		{
			bShowMouseCursor = bIsShowingInventory;
			
			FInputModeUIOnly InputMode;
						
			SetInputMode(InputMode);
			
		}
		else
		{
			bShowMouseCursor = bIsShowingInventory;
						
			FInputModeGameAndUI InputMode;

			SetInputMode(InputMode);			
			
		}

		PlayerUI->ShowInventoryPanel(bIsShowingInventory, InventoryComp);
	}
}
