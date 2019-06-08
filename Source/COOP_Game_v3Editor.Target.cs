// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class COOP_Game_v3EditorTarget : TargetRules
{
	public COOP_Game_v3EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "COOP_Game_v3" } );
	}
}
