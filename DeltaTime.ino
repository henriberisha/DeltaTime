void calculateTime(int totalSeconds, int &years, int &months, int &days, int &hours, int &minutes, int &seconds)
{
  const int SECONDS_IN_MINUTE = 60;
  const int MINUTES_IN_HOUR = 60;
  const int HOURS_IN_DAY = 24;
  const int DAYS_IN_MONTH = 30; // Assuming each month has 30 days for simplicity
  const int MONTHS_IN_YEAR = 12;

  // Calculate years
  years = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH * MONTHS_IN_YEAR);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH * MONTHS_IN_YEAR);

  // Calculate months
  months = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY * DAYS_IN_MONTH);

  // Calculate days
  days = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY);

  // Calculate hours
  hours = totalSeconds / (SECONDS_IN_MINUTE * MINUTES_IN_HOUR);
  totalSeconds %= (SECONDS_IN_MINUTE * MINUTES_IN_HOUR);

  // Calculate minutes
  minutes = totalSeconds / SECONDS_IN_MINUTE;
  totalSeconds %= SECONDS_IN_MINUTE;

  // Seconds left
  seconds = totalSeconds;

  Serial.print("Years: ");
  Serial.println(years);

  Serial.print("Months: ");
  Serial.println(months);

  Serial.print("Days: ");
  Serial.println(days);

  Serial.print("Hours: ");
  Serial.println(hours);

  Serial.print("Minutes: ");
  Serial.println(minutes);

  Serial.print("Seconds: ");
  Serial.println(seconds);
}

String iso8601(int deltaSeconds, int years, int months, int days, int hours, int minutes, int seconds)
{
  if (deltaSeconds == 0)
  {
    return String(years) + "-" + padd(months) + "-" + padd(days) + "T" + padd(hours) + ":" + padd(minutes) + ":" + padd(seconds) + "Z";
  }

  int years_contained, months_contained, days_contained, hours_contained, minutes_contained, seconds_contained;

  calculateTime(deltaSeconds, years_contained, months_contained, days_contained, hours_contained, minutes_contained, seconds_contained);

  int add_seconds = seconds + seconds_contained;
  int new_seconds = add_seconds % 60;

  int add_minutes = minutes + minutes_contained + (add_seconds / 60);
  int new_minutes = add_minutes % 60;

  int add_hours = hours + hours_contained + (add_minutes / 60);
  int new_hours = add_hours % 24;

  int add_days = days + days_contained + (add_hours / 24);
  int new_days = add_days % 30;

  int add_months = months + months_contained + (add_days / 30);
  int new_months = add_months % 12;

  int new_years = years + years_contained + (new_months / 12);

  return String(new_years) + "-" + padd(new_months) + "-" + padd(new_days) + "T" + padd(new_hours) + ":" + padd(new_minutes) + ":" + padd(new_seconds) + "Z";
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  int totalSeconds = 8000; // Replace this with your desired number of seconds
  int years = 2023, months = 7, days = 26, hours = 15, minutes = 00, seconds = 30;

  Serial.print(iso8601(totalSeconds, years, months, days, hours, minutes, seconds));
}

void loop()
{
  // put code here
}

String padd(int value)
{
  if (value < 10)
    return "0" + String(value);
  else
    return String(value);
}
