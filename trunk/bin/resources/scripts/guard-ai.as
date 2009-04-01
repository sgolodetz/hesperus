/***
 * hesperus: guard-ai.as
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

State@ state;

class State
{
	void process(ScriptYoke@ yoke)
	{
		// No implementation needed - to be overriden in subclasses
	}
}

class TestState : State
{
	TestState(ScriptYoke@ yoke)
	{
		// TODO
	}

	void process(ScriptYoke@ yoke)
	{
		if(!yoke.subyoke_exists())
		{
			yoke.goto_position(14,6,6);
		}
		else if(!yoke.subyoke_active())
		{
			yoke.clear_subyoke();
			@state = @OtherState(yoke);
		}
	}
}

class OtherState : State
{
	OtherState(ScriptYoke@ yoke)
	{
		// TODO
	}

	void process(ScriptYoke@ yoke)
	{
		if(!yoke.subyoke_exists())
		{
			yoke.goto_position(25,20,6);
		}
	}
}

// TODO

void f() {}

void init(ScriptYoke@ yoke)
{
	@state = @TestState(yoke);
}

void process(ScriptYoke@ yoke)
{
	state.process(yoke);
}
