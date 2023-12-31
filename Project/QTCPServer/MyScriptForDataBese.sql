USE [payrolldb]
GO
/****** Object:  Table [dbo].[PaymentPeriod]    Script Date: 10/25/2022 1:22:34 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PaymentPeriod](
	[payment_id] [int] IDENTITY(1,1) NOT NULL,
	[payment_start] [smalldatetime] NULL,
	[payment_end] [smalldatetime] NULL,
	[total_net_pay] [bigint] NULL,
	[total_count_emp] [int] NULL,
	[company_id] [int] NULL,
	[payment_date] [smalldatetime] NULL,
 CONSTRAINT [PK_PaymentPeriod] PRIMARY KEY CLUSTERED 
(
	[payment_id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PaymentInfoDetail]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PaymentInfoDetail](
	[payment_period_id] [int] NOT NULL,
	[gross_pay] [float] NOT NULL,
	[net_pay] [float] NOT NULL,
	[employee_id] [varchar](50) NOT NULL,
	[total_hours] [int] NOT NULL,
	[taxes] [int] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[employee]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[employee](
	[Employee_ID] [bigint] IDENTITY(1,1) NOT NULL,
	[ID] [varchar](50) NOT NULL,
	[Name] [ntext] NOT NULL,
	[DOB] [datetime] NOT NULL,
	[Gender] [ntext] NOT NULL,
	[Father] [ntext] NOT NULL,
	[Email] [ntext] NOT NULL,
	[Phone] [ntext] NOT NULL,
	[Address] [ntext] NOT NULL,
	[Department] [ntext] NOT NULL,
	[Designation] [ntext] NOT NULL,
	[DOJ] [datetime] NOT NULL,
	[Salary] [bigint] NOT NULL,
	[Type] [ntext] NOT NULL,
	[company_id] [int] NULL,
 CONSTRAINT [Employee_PRIMARY] PRIMARY KEY NONCLUSTERED 
(
	[ID] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[CompanySettings]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[CompanySettings](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Company_Name] [varchar](50) NULL,
	[Taxe] [int] NULL,
 CONSTRAINT [PK_CompanySettings] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  UserDefinedFunction [dbo].[getDataForPdfReportPayment]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Kozliuk Dmytro>
-- Create date: <10-24-2022 10:14pm>
-- Description:	<For PDF generate report>
-- =============================================
CREATE FUNCTION [dbo].[getDataForPdfReportPayment]
(	
	@param varchar(50)
)
RETURNS TABLE 
AS
RETURN 
(
	SELECT dbo.CompanySettings.Company_Name, 
	dbo.PaymentPeriod.payment_start, 
	dbo.PaymentPeriod.payment_end, 
	dbo.PaymentPeriod.payment_date, 
	dbo.employee.Name, 
	dbo.employee.Father, 
	dbo.employee.Gender, 
	dbo.employee.Email, 
	dbo.employee.ID,
	dbo.employee.Phone, 
	dbo.employee.Address,
	dbo.employee.Department, 
	dbo.employee.Designation,
	dbo.employee.DOJ, 
	dbo.employee.Salary, 
	dbo.PaymentInfoDetail.total_hours,
	dbo.PaymentInfoDetail.gross_pay, 
	dbo.PaymentInfoDetail.taxes,
	dbo.PaymentInfoDetail.net_pay
	FROM  dbo.CompanySettings INNER JOIN dbo.PaymentPeriod 
	ON dbo.CompanySettings.ID = dbo.PaymentPeriod.company_id 
	INNER JOIN dbo.PaymentInfoDetail
	ON dbo.PaymentPeriod.payment_id = dbo.PaymentInfoDetail.payment_period_id
	INNER JOIN dbo.employee 
	ON dbo.CompanySettings.ID = dbo.employee.company_id
	AND dbo.PaymentInfoDetail.employee_id = dbo.employee.ID
	WHERE dbo.employee.ID = @param
)
GO
/****** Object:  Table [dbo].[adminLogIn]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[adminLogIn](
	[adminLogin] [varchar](50) NOT NULL,
	[adminPass] [varchar](50) NULL,
	[role] [varchar](50) NULL,
	[company_id] [int] NULL,
 CONSTRAINT [PK_adminLogIn_1] PRIMARY KEY CLUSTERED 
(
	[adminLogin] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[AutorizationUser]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AutorizationUser](
	[Admin_id] [varchar](50) NOT NULL,
	[Unique_user_id] [bigint] NOT NULL,
	[Socket_descriptor] [int] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[company]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[company](
	[Department] [varchar](50) NOT NULL,
	[Designation] [varchar](50) NOT NULL,
	[Salary] [ntext] NOT NULL,
	[Dept_Short] [varchar](50) NOT NULL,
	[Design_Short] [varchar](50) NOT NULL,
	[Taxe] [int] NULL,
	[company_id] [int] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Employee_Attandance]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Employee_Attandance](
	[Employee_ID] [varchar](50) NOT NULL,
	[EnterDate] [datetime] NULL,
	[LeftDate] [datetime] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SystemSettings]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SystemSettings](
	[StartPeriodPayment] [datetime] NULL,
	[NextPayment] [datetime] NULL,
	[AutoPilot] [bit] NULL,
	[LastPayment] [datetime] NULL,
	[Frequency] [int] NULL,
	[Configuration] [bit] NULL,
	[company_id] [int] NULL
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[adminLogIn] ADD  CONSTRAINT [DF_adminLogIn_company_id]  DEFAULT ((1)) FOR [company_id]
GO
ALTER TABLE [dbo].[employee] ADD  DEFAULT ((0)) FOR [Salary]
GO
ALTER TABLE [dbo].[employee] ADD  CONSTRAINT [DF_employee_company_id]  DEFAULT ((1)) FOR [company_id]
GO
ALTER TABLE [dbo].[PaymentPeriod] ADD  CONSTRAINT [DF_PaymentPeriod_company_id]  DEFAULT ((1)) FOR [company_id]
GO
ALTER TABLE [dbo].[PaymentPeriod] ADD  CONSTRAINT [DF_PaymentPeriod_payment_date]  DEFAULT (getdate()) FOR [payment_date]
GO
ALTER TABLE [dbo].[SystemSettings] ADD  CONSTRAINT [DF_SystemSettings_company_id]  DEFAULT ((1)) FOR [company_id]
GO
ALTER TABLE [dbo].[adminLogIn]  WITH CHECK ADD  CONSTRAINT [FK_adminLogIn_CompanySettings] FOREIGN KEY([company_id])
REFERENCES [dbo].[CompanySettings] ([ID])
GO
ALTER TABLE [dbo].[adminLogIn] CHECK CONSTRAINT [FK_adminLogIn_CompanySettings]
GO
ALTER TABLE [dbo].[AutorizationUser]  WITH CHECK ADD  CONSTRAINT [FK_AutorizationUser_adminLogIn] FOREIGN KEY([Admin_id])
REFERENCES [dbo].[adminLogIn] ([adminLogin])
GO
ALTER TABLE [dbo].[AutorizationUser] CHECK CONSTRAINT [FK_AutorizationUser_adminLogIn]
GO
ALTER TABLE [dbo].[company]  WITH CHECK ADD  CONSTRAINT [FK_company_CompanySettings] FOREIGN KEY([company_id])
REFERENCES [dbo].[CompanySettings] ([ID])
GO
ALTER TABLE [dbo].[company] CHECK CONSTRAINT [FK_company_CompanySettings]
GO
ALTER TABLE [dbo].[employee]  WITH CHECK ADD  CONSTRAINT [FK_employee_CompanySettings] FOREIGN KEY([company_id])
REFERENCES [dbo].[CompanySettings] ([ID])
GO
ALTER TABLE [dbo].[employee] CHECK CONSTRAINT [FK_employee_CompanySettings]
GO
ALTER TABLE [dbo].[Employee_Attandance]  WITH CHECK ADD  CONSTRAINT [FK_Employee_Attandance_employee] FOREIGN KEY([Employee_ID])
REFERENCES [dbo].[employee] ([ID])
GO
ALTER TABLE [dbo].[Employee_Attandance] CHECK CONSTRAINT [FK_Employee_Attandance_employee]
GO
ALTER TABLE [dbo].[PaymentInfoDetail]  WITH CHECK ADD  CONSTRAINT [FK_PaymentInfoDetail_employee] FOREIGN KEY([employee_id])
REFERENCES [dbo].[employee] ([ID])
GO
ALTER TABLE [dbo].[PaymentInfoDetail] CHECK CONSTRAINT [FK_PaymentInfoDetail_employee]
GO
ALTER TABLE [dbo].[PaymentInfoDetail]  WITH CHECK ADD  CONSTRAINT [FK_PaymentInfoDetail_PaymentPeriod] FOREIGN KEY([payment_period_id])
REFERENCES [dbo].[PaymentPeriod] ([payment_id])
GO
ALTER TABLE [dbo].[PaymentInfoDetail] CHECK CONSTRAINT [FK_PaymentInfoDetail_PaymentPeriod]
GO
ALTER TABLE [dbo].[PaymentPeriod]  WITH CHECK ADD  CONSTRAINT [FK_PaymentPeriod_CompanySettings] FOREIGN KEY([company_id])
REFERENCES [dbo].[CompanySettings] ([ID])
GO
ALTER TABLE [dbo].[PaymentPeriod] CHECK CONSTRAINT [FK_PaymentPeriod_CompanySettings]
GO
ALTER TABLE [dbo].[SystemSettings]  WITH CHECK ADD  CONSTRAINT [FK_SystemSettings_CompanySettings] FOREIGN KEY([company_id])
REFERENCES [dbo].[CompanySettings] ([ID])
GO
ALTER TABLE [dbo].[SystemSettings] CHECK CONSTRAINT [FK_SystemSettings_CompanySettings]
GO
/****** Object:  StoredProcedure [dbo].[storeDataToPaymentPeriod]    Script Date: 10/25/2022 1:22:35 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Kozliuk Dmytro>
-- Create date: <22-10-2022>
-- Description:	<Procedure for store paymnet into table payment period>
-- =============================================
CREATE PROCEDURE [dbo].[storeDataToPaymentPeriod]
	--parameters for the stored procedure here
	@startPeriod smalldatetime,
	@endPeriod smalldatetime
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


	INSERT INTO dbo.PaymentPeriod(payment_start, payment_end) VALUES 
	(@startPeriod, @endPeriod);
	
INSERT INTO [payrolldb].[dbo].PaymentInfoDetail
	SELECT  (SELECT TOP(1)  payment_id FROM PaymentPeriod 
	 	 WHERE payment_start = @startPeriod
	AND payment_end = @endPeriod) AS ID_PERIOD, (employee.Salary * SUM(DATEDIFF(hour, EnterDate, LeftDate))) AS GrossPay,

((employee.Salary * SUM(DATEDIFF(hour, EnterDate, LeftDate))) - 
(((employee.Salary * SUM(DATEDIFF(hour, EnterDate, LeftDate))) 
* (SELECT TOP 1(company.Taxe) FROM company ))/100)) AS NetPay,

Employee_Attandance.Employee_ID AS Emp_ID,

SUM(DATEDIFF(hour, EnterDate, LeftDate)) AS Total_Hours,

(((employee.Salary * SUM(DATEDIFF(hour, EnterDate, LeftDate))) 
* (SELECT TOP 1(company.Taxe) FROM company ))/100) AS Taxes 

FROM dbo.Employee_Attandance  JOIN dbo.employee
ON Employee_Attandance.Employee_ID = employee.ID
WHERE EnterDate >= (@startPeriod)
AND LeftDate <= (@endPeriod) 
GROUP BY Employee_Attandance.Employee_ID, employee.Salary

DECLARE @payID int;
DECLARE @cnt_emp int;
DECLARE @total_net float;

SELECT @payID = payment_id FROM PaymentPeriod 
      WHERE payment_start = @startPeriod
  AND payment_end = @endPeriod

  SELECT @cnt_emp =  COUNT(payment_period_id) 
	FROM dbo.PaymentInfoDetail
	WHERE payment_period_id = @payID

	
	SELECT @total_net =  SUM(net_pay) 
		FROM dbo.PaymentInfoDetail
		WHERE payment_period_id = @payID

  UPDATE dbo.PaymentPeriod 
	SET 
	total_net_pay = @total_net,
	total_count_emp = @cnt_emp
	WHERE payment_id = @payID


END
GO
