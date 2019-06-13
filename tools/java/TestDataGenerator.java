import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.Writer;
import java.io.IOException;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.OffsetDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.time.zone.ZoneOffsetTransition;
import java.time.zone.ZoneRules;
import java.time.zone.ZoneRulesException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * Generate the validation_data.* files for AceTime.
 *
 * {@code
 * $ javac TestDataGenerator.java
 * $ java TestDataGenerator --scope (basic | extended) --startYear {start} --endYear {end}
 * }
 */
public class TestDataGenerator {
  // Number of seconds from Unix epoch (1970-01-01T00:00:00Z) to AceTime epoch
  // (2000-01-01T00:00:00Z).
  private static final int SECONDS_SINCE_UNIX_EPOCH = 946684800;

  public static void main(String[] argv) throws IOException {
    // Parse command line flags
    int argc = argv.length;
    int argi = 0;
    if (argc == 0) {
      usageAndExit();
    }
    String scope = null;
    String start = "2000";
    String end = "2050";
    while (argc > 0) {
      String arg0 = argv[argi];
      if ("--scope".equals(arg0)) {
        {argc--; argi++; arg0 = argv[argi];} // shift-left
        if (argc == 0) usageAndExit();
        scope = arg0;
      } else if ("--startYear".equals(arg0)) {
      } else if ("--endYear".equals(arg0)) {
      } else if ("--".equals(arg0)) {
        break;
      } else if (arg0.startsWith("-")) {
        System.err.printf("Unknown flag '%s'%n", arg0);
        usageAndExit();
      } else if (!arg0.startsWith("-")) {
        break;
      }
      {argc--; argi++;} // shift-left
    }
    if (!"basic".equals(scope) && !"extended".equals(scope)) {
      System.err.printf("Unknown scope '%s'%n", scope);
      usageAndExit();
    }
    // Should check for NumberFormatException but too much overhead for this simple tool
    int startYear = Integer.parseInt(start);
    int endYear = Integer.parseInt(end);

    List<String> zones = readZones();
    TestDataGenerator generator = new TestDataGenerator(scope, startYear, endYear, zones);
    generator.process();
  }

  private static void usageAndExit() {
    System.err.println("Usage: java TestDataGenerator --scope (basic|extended)");
    System.err.println("       [--startYear {start}] [--endYear {endYear}] < zones.txt");
    System.exit(1);
  }

  // Print out the list of all ZoneIds
  static void printAvailableZoneIds() {
    Set<String> allZones = ZoneId.getAvailableZoneIds();
    System.out.printf("Found %s ids total:\n", allZones.size());
    SortedSet<String> selectedIds = new TreeSet<>();
    int numZones = 0;
    for (String id : allZones) {
      if (id.startsWith("Etc")) continue;
      if (id.startsWith("SystemV")) continue;
      if (id.startsWith("US")) continue;
      if (id.startsWith("Canada")) continue;
      if (id.startsWith("Brazil")) continue;
      if (!id.contains("/")) continue;
      selectedIds.add(id);
    }
    System.out.printf("Selected %s ids:\n", selectedIds.size());
    for (String id : selectedIds) {
      System.out.println("  " + id);
    }
  }

  /** Return the list of zone names from the System.in. */
  private static List<String> readZones() throws IOException {
    List<String> zones = new ArrayList<>();
    try (BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
      String line;
      while ((line = reader.readLine()) != null) {
        zones.add(line);
      }
    }
    return zones;
  }

  private TestDataGenerator(String scope, int startYear, int endYear, List<String> zones) {
    this.scope = scope;
    this.startYear = startYear;
    this.endYear = endYear;
    this.zones = zones;

    if (scope == "basic") {
      this.cppFile = "validation_data.cpp";
      this.headerFile = "validation_data.h";
      this.dbNamespace = "zonedb";
    } else {
      this.cppFile = "validation_data.cpp";
      this.headerFile = "validation_data.h";
      this.dbNamespace = "zonedbx";
    }
  }

  private void process() throws IOException {
    Map<String, List<TestItem>> testData = new TreeMap<>();
    for (String zoneName : zones) {
      ZoneId zoneId;
      try {
        zoneId = ZoneId.of(zoneName);
      } catch (ZoneRulesException e) {
        System.out.printf("Zone '%s' not found%n", zoneName);
        continue;
      }
      List<TestItem> testItems = createValidationData(zoneId);
      testData.put(zoneName, testItems);
    }

    printCpp(testData);
    printHeader(testData);
  }

  private void printCpp(Map<String, List<TestItem>> testData) throws IOException {
    try (PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(cppFile)))) {
      writer.println("#include <AceTime.h>");
      writer.println("#include \"validation_data.h\"");
      writer.println("namespace ace_time {");
      writer.printf("namespace %s {%n", dbNamespace);

      for (Map.Entry<String, List<TestItem>> entry : testData.entrySet()) {
        String zoneName = entry.getKey();
        List<TestItem> testItems = entry.getValue();
        printDataToFile(writer, zoneName, testItems);
      }

      writer.println("}");
      writer.println("}");
    }

    System.out.printf("Created %s%n", cppFile);
  }

  private void printHeader(Map<String, List<TestItem>> testData) {
  }

  private void printDataToFile(PrintWriter writer, String zoneName, List<TestItem> testItems) {
    writer.println(zoneName);
    for (TestItem item : testItems) {
      writer.printf("  %d %d %d %d %d %d %d %c%n",
          item.epochSeconds,
          item.year,
          item.month,
          item.day,
          item.hour,
          item.minute,
          item.second,
          item.type);
    }
  }

  private List<TestItem> createValidationData(ZoneId zoneId) {
    Instant instant = LocalDateTime.of(startYear, 1, 1, 0, 0, 0)
        .toInstant(ZoneOffset.UTC);
    ZoneRules rules = zoneId.getRules();
    List<TestItem> testItems = new ArrayList<>();
    while (true) {
      ZoneOffsetTransition transition = rules.nextTransition(instant);
      if (transition == null) {
        break;
      }
      instant = transition.getInstant();
      LocalDateTime dt = LocalDateTime.ofInstant(instant, zoneId);
      if (dt.getYear() > endYear) {
        break;
      }

      Instant instantBefore = instant.minusSeconds(1);
      ZoneOffset offsetBefore = transition.getOffsetBefore();
      TestItem item = createTestItem(instantBefore, offsetBefore, 'A');
      testItems.add(item);

      ZoneOffset offsetAfter = transition.getOffsetAfter();
      item = createTestItem(instant, offsetAfter, 'B');
      testItems.add(item);
    }

    return testItems;
  }

  private TestItem createTestItem(Instant instant, ZoneOffset offset, char type) {
    LocalDateTime ldt = LocalDateTime.ofInstant(instant, offset);
    TestItem item = new TestItem();

    item.epochSeconds = (int) (instant.toEpochMilli() / 1000 - SECONDS_SINCE_UNIX_EPOCH);
    item.utcOffset = offset.getTotalSeconds();
    item.year = ldt.getYear();
    item.month = ldt.getMonthValue();
    item.day = ldt.getDayOfMonth();
    item.hour = ldt.getHour();
    item.minute = ldt.getMinute();
    item.second = ldt.getSecond();
    item.type = type;

    return item;
  }

  private final String scope;
  private final List<String> zones;

  private final String cppFile;
  private final String headerFile;
  private final String dbNamespace;

  private final int startYear;
  private final int endYear;
}

class TestItem {
  int epochSeconds;
  int utcOffset;
  int dstOffset;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  char type;
}
