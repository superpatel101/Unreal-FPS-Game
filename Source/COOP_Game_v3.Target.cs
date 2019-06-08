// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class COOP_Game_v3Target : TargetRules
{
	public COOP_Game_v3Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "COOP_Game_v3" } );
	}
}
