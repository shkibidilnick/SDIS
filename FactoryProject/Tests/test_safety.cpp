#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Safety/SafetyIncident.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Safety/SafetyTraining.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/SafetyException.h"

// ==================== Тесты SafetyIncident ====================
TEST(SafetyIncident_Constructor_CreatesIncidentWithCorrectProperties) {
  SafetyIncident incident("INJURY", "Worker cut finger on sharp edge",
                          "Station 5", "EMP001");

  CHECK(!incident.getTrackingId().empty());
  CHECK_EQUAL("INJURY", incident.getIncidentType());
  CHECK_EQUAL("Worker cut finger on sharp edge", incident.getDescription());
  CHECK_EQUAL("Station 5", incident.getLocation());
  CHECK_EQUAL("EMP001", incident.getReportedBy());
  CHECK_EQUAL("MEDIUM", incident.getSeverity());
  CHECK_EQUAL(false, incident.getIsInvestigated());
  CHECK_EQUAL("OPEN", incident.getStatus());
}

TEST(SafetyIncident_Investigate_SetsInvestigationReport) {
  SafetyIncident incident("NEAR_MISS", "Near miss with forklift", "Warehouse",
                          "EMP002");

  incident.investigate("Investigation completed. Root cause: operator error.");

  CHECK_EQUAL(true, incident.getIsInvestigated());
  CHECK_EQUAL("Investigation completed. Root cause: operator error.",
              incident.getInvestigationReport());
}

TEST(SafetyIncident_Investigate_ThrowsForEmptyReport) {
  SafetyIncident incident("EQUIPMENT_DAMAGE", "Machine overheated", "Line 3",
                          "EMP003");

  CHECK_THROW(incident.investigate(""), SafetyException);
}

TEST(SafetyIncident_Close_ClosesIncident) {
  SafetyIncident incident("INJURY", "Minor burn", "Kitchen", "EMP005");

  incident.investigate("Burn from hot surface. Add warning signs.");
  incident.addCorrectiveAction("Install heat-resistant barriers");
  incident.close();

  CHECK_EQUAL("CLOSED", incident.getStatus());
}

TEST(SafetyIncident_Close_ThrowsWhenNotInvestigated) {
  SafetyIncident incident("NEAR_MISS", "Slip hazard", "Entrance", "EMP006");

  // Не проводим расследование
  CHECK_THROW(incident.close(), SafetyException);
}

TEST(SafetyIncident_Close_ThrowsWithoutCorrectiveActions) {
  SafetyIncident incident("INJURY", "Cut from tool", "Workshop", "EMP007");

  incident.investigate("Tool mishandling");
  // Не добавляем corrective actions

  CHECK_THROW(incident.close(), SafetyException);
}

// ==================== Тесты SafetyTraining ====================
TEST(SafetyTraining_Constructor_CreatesTrainingWithCorrectProperties) {
  SafetyTraining training("Forklift Operation", 24);

  CHECK(!training.getTrainingId().empty());
  CHECK_EQUAL("Forklift Operation", training.getCertificationName());
  CHECK_EQUAL(24, training.getValidityPeriodMonths());
  CHECK_EQUAL(true, training.getRequiresRenewal());
}

TEST(SafetyTraining_CertifyEmployee_AddsEmployeeToCertifiedList) {
  SafetyTraining training("Hazmat Handling", 12);

  training.certifyEmployee("EMP001");

  CHECK_EQUAL(true, training.isEmployeeCertified("EMP001"));
  CHECK_EQUAL(false, training.isEmployeeCertified("EMP002"));
}

TEST(SafetyTraining_CertifyEmployee_ThrowsWhenAlreadyCertified) {
  SafetyTraining training("Safety Basics", 6);

  training.certifyEmployee("EMP001");
  CHECK_THROW(training.certifyEmployee("EMP001"), SafetyException);
}

TEST(SafetyTraining_RevokeCertification_RemovesEmployee) {
  SafetyTraining training("Equipment Operation", 12);

  training.certifyEmployee("EMP001");
  training.certifyEmployee("EMP002");
  training.revokeCertification("EMP001");

  CHECK_EQUAL(false, training.isEmployeeCertified("EMP001"));
  CHECK_EQUAL(true, training.isEmployeeCertified("EMP002"));
}

TEST(SafetyTraining_RevokeCertification_ThrowsWhenNotCertified) {
  SafetyTraining training("Safety Training", 12);

  CHECK_THROW(training.revokeCertification("EMP999"), SafetyException);
}

TEST(SafetyTraining_GetDaysUntilExpiration_ReturnsCorrectDays) {
  SafetyTraining training("Basic Safety", 1);

  training.certifyEmployee("EMP001");
  int days = training.getDaysUntilExpiration("EMP001");

  CHECK(days > 25 && days <= 31);
}

TEST(SafetyTraining_GetDaysUntilExpiration_ThrowsWhenNotCertified) {
  SafetyTraining training("Specialized Training", 12);

  CHECK_THROW(training.getDaysUntilExpiration("EMP999"), SafetyException);
}
