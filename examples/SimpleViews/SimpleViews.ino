#include <ArdUI.h>
#include <ViewGroup.h>
#include <views/SeekBar.h>
#include <views/TextView.h>
#include <views/CheckBox.h>
#include <views/Button.h>

#include <UTFT.h>
#include <UTouch.h>
#include <DueTimer.h>

int InterruptPin = 2;

// Views
ViewGroup *holder;

Button *btn;
CheckBox *check;
SeekBar *skbr;
TextView *txtView;

// LCD and Touch objects
UTFT myLCD(CTE50,25,26,27,28);
UTouch myTCH = UTouch(6,5,32,3,InterruptPin);

// Fonts
extern uint8_t BigFont[];

// Callbacks
void confirm(){
	Serial.println("Confirm");
}

void changedValue(bool value){
	Serial.println(value ? "Checked" : "Unchecked");
}

void selectValue(long value){
	Serial.print("Changed value: ");
	Serial.println(value);	
}

void setup(){

	Serial.begin(9600);

	// UTFT and UTouch initialization
	myLCD.InitLCD();
	myLCD.clrScr();
	myLCD.setFont(BigFont);

	myTCH.InitTouch(LANDSCAPE);
    myTCH.setPrecision(PREC_MEDIUM);

	// Initialize ArdUI
	ArdUI::touchInterrupt = InterruptPin;		// Interrupt pin of Touch
    ArdUI::touchTimer =  &Timer1;				// Configure the Timer
    ArdUI::touchObject = &myTCH;				// Configure the Touch
    ArdUI::touchMode = ArdUI::TIMER_ONLY;	// For compatibility with latest UTFT and UTouch libraries
    ArdUI::touchTimerPeriod  = 50000;			// 50ms of period
    ArdUI::LCD = &myLCD; 						// Configure our LCD

	// Buton setup
	btn = new Button();
	btn->name = "OK";
	btn->w = 100;
	btn->h = 50;
	btn->onClick(confirm);
	btn->o = Point(20,20);

	// Checkbox setup
	check = new CheckBox();
	check->name = "Enable?";
	check->onChange(changedValue);
	check->setChecked(true, false);  // Let's start our checkbox already checked
									// false, is to not render now
	check->o.x = 20; // set relative x position
	check->o.y = 100; // set relative y position
	check->h = 30;   // Height is now 40
	check->w = 30;   // Width is now 40

	// SeekBar
	skbr = new SeekBar();
	skbr->o = Point(40,150);
	skbr->setMax(150);
	skbr->setMin(20);
	skbr->onChange(selectValue);

	// TextView
	txtView = new TextView();
	txtView->setValue("-", false); // false, to not render now
	txtView->o = Point(20,200);

    // ViewGroup setup
    holder = new ViewGroup();

    holder->addView(btn); // Remember to always pass the pointer
    holder->addView(check);
    holder->addView(skbr);
    holder->addView(txtView);

    // Now, we need to set the root view for ArdUI check touches
    ArdUI::rootView = holder;

    // Enable touch
    ArdUI::enableTouch();

    // Render all views
    holder->render(true); // true, to force rendering
}

void loop(){
	while(true){
		// Don't do anything, and views will continue to work, callbacks also
		delay(1000);
		txtView->setValue(String(millis()));
	}
}
