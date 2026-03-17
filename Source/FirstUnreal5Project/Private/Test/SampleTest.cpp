// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSampleTest, "UE5 Functional Project.SampleTest.Test Sample Function -- Should Return True", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FSampleTest::RunTest(const FString& Parameters)
{
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
