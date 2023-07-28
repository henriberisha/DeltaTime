/**
 * @author henriberisha
 * @date July 28th, 2023
 *
 * Free to use
 * Platforms:
 *    - Arduino,
 *    - ESP
 *
 * I put a lot of comments to explain in detail what the code is doing
 * Miliseconds are not handled, they will be zero
 * Only future dates can be calculated, deltaSeconds >= 0
 * Negative delta is not handled, cannot find past dates
 * If any argument is inputed as a float, it will be casted to closest (floor) int
 */

String padd(int valueDigit)
/**
  Pads single digit with a zero
  applies to month[1-12], day[1-30], hour[0-23], minute[0-59], second[0-59]
  year is taken for granted to be with 4 digits

  @param valueDigit single digit from either m, d, h, m, or s

  @return padded value of single digit with a zero in front
*/

{
  if (valueDigit < 10)
    return "0" + String(valueDigit);
  else
    return String(valueDigit);
}

void calculateTimeBreakdown(long totalSeconds, int &years, int &months, int &days, int &hours, int &minutes, int &seconds)
{
  /**
    Calculates the ammount of years, months, days, hours, minutes, and seconds that are contained in the 'totalSeconds'

    @param totalSeconds total seconds to convert in years, months, days, hours, minutes, and seconds
    @param years variable to store the years into (by reference)
    @param months variable to store the months into (by reference)
    @param days variable to store the days into (by reference)
    @param hours variable to store the hours into (by reference)
    @param minutes variable to store the minutes into (by reference)
    @param seconds variable to store the seconds into (by reference)

    @return void; only prints to Serial output

  */

  Serial.println(String(totalSeconds) + " seconds contain:");

  const int SECONDS_IN_MINUTE = 60;
  const int MINUTES_IN_HOUR = 60;
  const int HOURS_IN_DAY = 24;
  const int DAYS_IN_MONTH = 30; // Arbitrary choosing month has 30 days
  const int MONTHS_IN_YEAR = 12;

  // Calculate years by dividing 'totalSeconds' by how many seconds are contained in ONE year
  // Update 'totalSeconds' to be equal to the remainder of the seconds
  years = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH * MONTHS_IN_YEAR);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH * MONTHS_IN_YEAR);

  // Calculate months by dividing 'totalSeconds' by how many seconds are contained in ONE month
  // Update 'totalSeconds' to be equal to the remainder of the seconds
  months = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH);

  // Calculate days by dividing 'totalSeconds' by how many seconds are contained in ONE day
  // Update 'totalSeconds' to be equal to the remainder of the seconds
  days = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY);

  // Calculate hours by dividing 'totalSeconds' by how many seconds are contained in ONE hour
  // Update 'totalSeconds' to be equal to the remainder of the seconds
  hours = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR);

  // Calculate minutes by dividing 'totalSeconds' by how many seconds are contained in ONE minute
  // Update 'totalSeconds' to be equal to the remainder of the seconds
  minutes = totalSeconds / SECONDS_IN_MINUTE;
  totalSeconds %= SECONDS_IN_MINUTE;

  // Seconds left
  seconds = totalSeconds;

  Serial.println("Years: " + String(years));
  Serial.println("Months: " + String(months));
  Serial.println("Days: " + String(days));
  Serial.println("Hours: " + String(hours));
  Serial.println("Minutes: " + String(minutes));
  Serial.println("Seconds: " + String(seconds));
}

String time_ISO8601(long deltaSeconds, int year, int month, int day, int hour, int minute, int second)
{
  /**
    Produces the future UTC date-time after 'deltaSeconds' have passed
    It is assumed that input and output will be UTC date-time; change accordingly to your preferences
    Miliseconds are never handled and they appear zero as '.00'


    @param deltaSeconds difference of seconds into the future to calculate the new UTC date-time
    @param year actual/present year from UTC date-time (NOW)
    @param month actual/present month from UTC date-time (NOW)
    @param day actual/present day from UTC date-time (NOW)
    @param hour actual/present hour from UTC date-time (NOW)
    @param minute actual/present minute from UTC date-time (NOW)
    @param second actual/present second from UTC date-time (NOW)

    @return future date-time after 'deltaSeconds' have passed from the present date-time

  */

  //  If no seconds have passed, return the present date-time
  if (deltaSeconds == 0)
  {
    return String(year) + "-" + padd(month) + "-" + padd(day) + "T" + padd(hour) + ":" + padd(minute) + ":" + padd(second) + ".00Z";
  }

  int years_contained, months_contained, days_contained, hours_contained, minutes_contained, seconds_contained;

  //  Calculate the contained y,m,d,h,m,s in 'deltaSeconds'
  calculateTimeBreakdown(deltaSeconds, years_contained, months_contained, days_contained, hours_contained, minutes_contained, seconds_contained);

  //  Perform date-time arithmetic and handle overflow
  //  i.e. 'add_seconds' = 82  =>  there is overflow of 22 seconds
  //  this will be 1 minute and 22 seconds
  //  here 'new_second' will be 22 (82 % 60) and 1 minute will be added in the bottom code (add_seconds / 60)
  //  SAME MATH LOGIC FOR THE REST OF THE CODE
  int add_seconds = second + seconds_contained;
  int new_second = add_seconds % 60;

  int add_minutes = minute + minutes_contained + (add_seconds / 60);
  int new_minute = add_minutes % 60;

  int add_hours = hour + hours_contained + (add_minutes / 60);
  int new_hour = add_hours % 24;

  int add_days = day + days_contained + (add_hours / 24);
  int new_day = add_days % 30;

  int add_months = month + months_contained + (add_days / 30);
  int new_month = add_months % 12;

  int new_year = year + years_contained + (add_months / 12);

  //  return the future date-time as UTC
  return String(new_year) + "-" + padd(new_month) + "-" + padd(new_day) + "T" + padd(new_hour) + ":" + padd(new_minute) + ":" + padd(new_second) + ".00Z";
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  //  Basic, hardcoded test cases

  //  Approximate time when this script was completed
  //  Date-time: "2023-07-28T05:30:40.00Z"; find the future date-time when 8575 seconds have passed
  long deltaSeconds = 8575; // Replace this with your desired number of seconds
  int year_now = 2023, month_now = 7, day_now = 28, hour_now = 5, minute_now = 30, second_now = 40;

  //  This will return the input date-time because delta is zero; no seconds have passed
  Serial.println("INPUT TIME: " + time_ISO8601(0, year_now, month_now, day_now, hour_now, minute_now, second_now));
  Serial.println("FUTURE TIME: " + time_ISO8601(deltaSeconds, year_now, month_now, day_now, hour_now, minute_now, second_now));

  Serial.println("\n");

  //  setup for randomized test cases
  //  if analog input pin 0 is unconnected, random analog
  //  noise will cause the call to randomSeed() to generate
  //  different seed numbers each time the sketch runs.
  //  randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  Serial.println("Randomized test cases start now...\n");
}

void loop()
{
  Serial.println("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

  //  adjust max to lower as needed
  long deltaSeconds = random(0, 2147483640); // max is bound by random function that can take up to 2147483647  ~ 68 years

  int year_now = random(1970, 3000); // year generated randomly between [1970, 3000)
  int month_now = random(1, 13);     // month generated randomly between [1, 13)
  int day_now = random(1, 31);       // day generated randomly between [1, 31)
  int hour_now = random(0, 24);      // hour generated randomly between [0, 24)
  int minute_now = (0, 60);          // minute generated randomly between [0, 60)
  int second_now = (0, 60);          // second generated randomly between [0, 60)

  Serial.println("INPUT TIME: " + time_ISO8601(0, year_now, month_now, day_now, hour_now, minute_now, second_now));
  Serial.println("FUTURE TIME: " + time_ISO8601(deltaSeconds, year_now, month_now, day_now, hour_now, minute_now, second_now));

  delay(10000); // wait 10 seconds
}
