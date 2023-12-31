/****** Object:  StoredProcedure [dbo].[uspAddUser]  ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[uspAddUser]
    @pLogin NVARCHAR(50), 
    @pPassword NVARCHAR(50),
    @companyId int, 
    @responseMessage NVARCHAR(250) OUTPUT
AS
BEGIN
    SET NOCOUNT ON

    DECLARE @salt UNIQUEIDENTIFIER=NEWID()
    BEGIN TRY

        INSERT INTO dbo.[adminLogIn] (adminLogin, adminPassHash, salt, company_id)
        VALUES(@pLogin, HASHBYTES('SHA2_512', @pPassword+CAST(@salt AS NVARCHAR(36))), @salt, @companyId)

       SET @responseMessage='Success'

    END TRY
    BEGIN CATCH
        SET @responseMessage=ERROR_MESSAGE() 
    END CATCH

END
GO
/****** Object:  StoredProcedure [dbo].[uspLogin]  ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[uspLogin]
    @pLoginName VARCHAR(254),
    @pPassword VARCHAR(50),
    @resp bit OUTPUT
AS
BEGIN

    SET NOCOUNT ON

    DECLARE @userID VARCHAR

    IF EXISTS (SELECT TOP 1 adminLogIn FROM [dbo].[adminLogIn] WHERE adminLogin = @pLoginName)
    BEGIN
        SET @userID=(SELECT adminLogIn FROM [dbo].[adminLogIn] WHERE adminLogin = @pLoginName AND adminPassHash  = HASHBYTES('SHA2_512', @pPassword+CAST(Salt AS NVARCHAR(36))))

       IF(@userID IS NULL)
            SET @resp = 0
       ELSE 
           SET @resp = 1
    END
    ELSE
        SET @resp = 0

END
GO
