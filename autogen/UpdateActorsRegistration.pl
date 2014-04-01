#!/usr/bin/perl -w
use strict;

#
# Globals
#

my $Added_registrations = 0;
my $Removed_registrations = 0;

my $REGISTRATION_START ='// ## This is automatic generated text. Pleace do not change it.';
my $REGISTRATION_END = '// ## End of automatic generated text';

my $REGISTRATION_TAG = '// ## Registration in ActorFactory';

#
# Action
#

print('Updating actors registration in actor factory: ');

my @registered_actors = ReadRegisteredActors('../configuration/RegisteredActors.txt');
my $dir='../src/';
UpdateAllClasses($dir, @registered_actors);

print('Added '.$Added_registrations.' Removed '.$Removed_registrations);

#
# Get class name from *.h file
#
sub GetActorClass #(class_file)
{
	my ($class_file) = @_;

	open(SOURCE, $class_file.'.h') || return "-1";
	my @text = <SOURCE>;
	close(SOURCE);

	my $all_lines = join("\n", @text);

	if ($all_lines=~m/class\s+(\w+)\s*:\s*public\s+/)
	{
		my $class_name = $1;
		return $class_name;
	}
	else
	{
		return "-1";
	}
};

#
# Return 1 if class already have registration in ActorFactory
#
sub IsAlreadyHaveRegistration #(class_file)
{
	my ($class_file) = @_;

	open(SOURCE, $class_file.'.cpp');
	my @text = <SOURCE>;
	close(SOURCE);

	my $all_lines = join("\n", @text);

	if ($all_lines=~m/## Registration in ActorFactory/)
	{
		return 1;
	}

	return 0;
};

#
# Include registration in actor factory in *.cpp file of class
#
sub AddAFRegistration #(class_file)
{
	my ($class_file) = @_;

	if (IsAlreadyHaveRegistration($class_file) == 1)
	{
		return 0;
	}

	my $class_name = GetActorClass($class_file);

	my $autotext =
		"\n\n".$REGISTRATION_START
		."\n".$REGISTRATION_TAG
		."\n#include \"../../Engine/Modules/ActorFactory.h\""
		."\n// unnamed namespase to hide from another places"
		."\nnamespace"
		."\n{"
		."\n	// specific factory"
		."\n	IActor* Create".$class_name."(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)"
		."\n	{"
		."\n		return new ".$class_name."(world, location, scale, rotation);"
		."\n	}"
		."\n"
		."\n	const std::string CLASS_ID = \"".$class_name."\";"
		."\n"
		."\n	// register specific factory in actor factory"
		."\n	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, Create".$class_name.");"
		."\n}"
		."\n".$REGISTRATION_END."\n\n";

	my $file_name = $class_file.'.cpp';

	open(SOURCE, $file_name);
	my @text = <SOURCE>;
	close(SOURCE);

	my $line_number = 0;

	my $i = 0;
	while ($i < @text)
	{
		# searching for first include string (#include "ActorClass.h")
		if ($text[$i]=~m/#include\s+\"$class_name\.h\"/)
		{
			$line_number = $i + 1;
		}
		$i++;
	}

	my @end = splice(@text, 0, $line_number);

	unshift(@text, @end, $autotext);

	open(SOURCE_2, '>'.$file_name) || die "Error with opening a file. $!";
	print SOURCE_2 @text;
	close(SOURCE_2);
	
	$Added_registrations++;
};

#
# Remove registration in actor factory from *.cpp file of class
#
sub RemoveAFRegistration #(class_file)
{
	my ($class_file) = @_;

	if (IsAlreadyHaveRegistration($class_file) == 0)
	{
		return 0;
	}

	my $class_name = GetActorClass($class_file);

	my $file_name = $class_file.'.cpp';

	open(SOURCE, $file_name);
	my @text = <SOURCE>;
	close(SOURCE);

	my $begin_line_number = -1;
	my $end_line_number = 0;
	my $in_block = 0;

	my $i = 0;
	while ($i < @text)
	{
		# searching for first include string (#include "ActorClass.h")
		if ($in_block == 0 && $text[$i]=~m/$REGISTRATION_START/)
		{
			if ($i < @text && $text[$i + 1]=~m/$REGISTRATION_TAG/)
			{
				$begin_line_number = $i;
				$in_block = 1;
			}
		}

		if ($in_block == 1 && $text[$i]=~m/$REGISTRATION_END/)
		{
			$end_line_number = $i;
			$in_block = 2
		}

		$i++;
	}

	splice(@text, $begin_line_number, $end_line_number);

	open(SOURCE_2, '>'.$file_name) || die "Error with opening a file. $!";
	print SOURCE_2 @text;
	close(SOURCE_2);
	
	$Removed_registrations++;
};

#
#
#
sub IsArrayConstrains #($value, @values)
{
	my ($value, @values) = @_;
	
	foreach my $cur_value(@values)
	{
		if ($value eq $cur_value)
		{
			return 1;
		}
	}
	
	return 0;
};

#
#
#
sub UpdateClass #($dir, @registered_actors)
{
	my ($class_path, @registered_actors) = @_;
	
	my $class_name = substr($class_path, rindex($class_path, '/') + 1);

	if (GetActorClass($class_path) eq "-1")
	{
		return;
	}
	
	if (IsArrayConstrains($class_name, @registered_actors) == 1)
	{
		AddAFRegistration($class_path);
	}
	else
	{
		RemoveAFRegistration($class_path);
	}
};

#
#
#
sub UpdateAllClasses #($dir, @registered_actors)
{
	my ($dir, @registered_actors) = @_;
	
	opendir DIR, $dir or return;
	my @contents = map "$dir/$_", sort grep !/^\.\.?$/, readdir DIR;
	closedir DIR;
	
	foreach my $file (@contents)
	{
		if (!-l $file && -d $file)
		{
			UpdateAllClasses($file, @registered_actors);
		}
		else
		{
			if ($file =~ /.cpp/i)
			{
				my $class_path = substr($file, 0, -4);
				UpdateClass($class_path, @registered_actors);
			}
		}
	}
};

#
# Read all lines in file contains information about registered actors
#
sub ReadRegisteredActors #(actors_file)
{
	my ($actors_file) = @_;

	open(ACTORS, $actors_file);
	my @actors = <ACTORS>;
	close(ACTORS);

	chomp @actors;

	return @actors;
};