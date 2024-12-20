// Copyright 2022, Aquanox.

#pragma once

#include "Templates/SharedPointer.h"
#include "Engine/World.h"

/**
 * Subsystem data provider delegate
 */
DECLARE_DELEGATE_TwoParams(FEnumSubsystemsDelegate, UWorld* /* InContext */, TArray<UObject*>& /* OutData */);

/**
 * Represents a type of subsystem that will be shown in browser
 */
struct SUBSYSTEMBROWSER_API FSubsystemCategory : public TSharedFromThis<FSubsystemCategory>
{
	/* Category config identifier */
	FName Name;
	/* Category display in Settings */
	FName SettingsName;
	/* Category display title */
	FText Label;
	/* Sort weight for the category (with 0 being topmost, 1000 bottom last) */
	int32 SortOrder = 0;

	FSubsystemCategory() = default;
	FSubsystemCategory(const FName& Name, const FText& Label, int32 SortOrder);
	virtual ~FSubsystemCategory() = default;

	const FName& GetID() const { return Name; }
	const FName& GetSettingsName() const { return SettingsName; }
	const FText& GetDisplayName() const { return Label; }
	int32 GetSortOrder() const { return SortOrder; }

	virtual bool IsVisibleByDefault() const { return true; }
	virtual bool IsVisibleInSettings() const { return true; }

	/* Get primary subsystem category class */
	virtual UClass* GetSubsystemClass() const;

	/* Select subsystems for this category */
	virtual void Select(UWorld* InContext, TArray<UObject*>& OutData) const = 0;
};

using SubsystemCategoryPtr = TSharedPtr<FSubsystemCategory>;

/**
 * Basic implementation of category that take in delegate selector
 */
struct SUBSYSTEMBROWSER_API FSimpleSubsystemCategory : public FSubsystemCategory
{
	/* Data supplier function */
	FEnumSubsystemsDelegate Selector;

	FSimpleSubsystemCategory() = default;
	FSimpleSubsystemCategory(const FName& Name, const FText& Label, const FEnumSubsystemsDelegate& Selector, int32 SortOrder);

	virtual void Select(UWorld* InContext, TArray<UObject*>& OutData) const override;
};
