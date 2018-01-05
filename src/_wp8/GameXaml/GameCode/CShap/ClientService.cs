using PhoneDirect3DXamlAppComponent;
using Pine.Object;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Data.Services.Client;
using System.Windows;
using System.Security.Cryptography;
using System.IO;
using Pine.WildWildWestServer;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Net;
namespace Pine {

    public class OdataServerDefine {
        public const string UrlServerReal = "http://wildwildwest.trafficmanager.net/api/WildWildWest";
        public const string UrlServerRealTest = "http://wildwildwest.trafficmanager.net/WATMmonitorfile.htm";
        public const string Secret = "pineEntertainment";
    }
    public class CurrentGameServiceRequest:ClientServiceRequest {
        public CurrentGameServiceRequest(int request_code)
            : base(request_code)
        {
        }
        public static ClientServiceRequest CheckServer = new ClientServiceRequest(1001);
        public static ClientServiceRequest CheckAccount = new ClientServiceRequest(1002);
        public static ClientServiceRequest CreateNewAccount = new ClientServiceRequest(1003);
        public static ClientServiceRequest SyncSaveToServer = new ClientServiceRequest(1004);
        public static ClientServiceRequest SyncSaveFromServer = new ClientServiceRequest(1005);

    }
    public class CurrentGameService:ClientService {
        public const int MAX_SAVE_BUFFER_CACHE = 10000;
        ServerStream _stream_check_server;
        ServerStream _stream_check_account;
        ServerStream _stream_save_game_waiting;

        private string Context_Url;

        public override string OuthorizeString(string id) {
            string rs = "";
            byte[] keybytes = new byte[32];
            string key = OdataServerDefine.Secret;
            byte[] tmp_keybytes = UTF8Encoding.UTF8.GetBytes(key);
            AesManaged aes = new AesManaged();
            byte[] iv = new byte[aes.BlockSize / 8];
            try {
                Array.Resize(ref tmp_keybytes, aes.KeySize / 8);
                keybytes = tmp_keybytes;
                aes.Key = keybytes;
                aes.IV = iv;
                ICryptoTransform aes_transform = aes.CreateEncryptor(aes.Key, aes.IV);
                // Create the streams used for encryption. 
                using (MemoryStream msEncrypt = new MemoryStream()) {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, aes_transform, CryptoStreamMode.Write)) {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt)) {
                            swEncrypt.Write(id);
                        }
                        rs = Convert.ToBase64String(msEncrypt.ToArray());
                    }
                }
            } catch (Exception e) {
            }
            return rs;
        }


        ODataService<WildWildWestServer.WildWildWestContext> _context_check_account = null;
        DataServiceCollection<WildWildWestServer.SaveGameDto> _collection_gamedto = null;

        


        public CurrentGameService()
        {
            
        }
        public override void process(ClientServiceRequest request, ServerStream svstream) {
            if (request == CurrentGameServiceRequest.CheckServer)
            {
                
                if(_stream_check_server==null)
                {
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_REQUESTING;
                    _stream_check_server = svstream;
                    CheckServerUrl();
                    
                }
                else
                {
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
            }
            else if (request == CurrentGameServiceRequest.CheckAccount)
            {
                if (_stream_check_account == null)
                {
                    _stream_check_account = svstream;
                    if (_context_check_account == null && Context_Url.Length > 0)
                    {
                        _context_check_account = new ODataService<WildWildWestServer.WildWildWestContext>(Context_Url);
                    }
                    if (_context_check_account != null)
                    {
                        StreamBuffer buffer = new StreamBuffer(svstream.PostData);
                        UInt64 userID = buffer.readUInt64();
                        _context_check_account.setUserId(userID);
                        _collection_gamedto = new DataServiceCollection<SaveGameDto>(_context_check_account._context);
                        IQueryable<SaveGameDto> query = from t in _context_check_account._context.SaveGames
                                                        where t.ID == (long)userID
                                                        select t;
                        _collection_gamedto.LoadCompleted += getHeaderComplete;
                        _collection_gamedto.LoadAsync(query);
                    }
                }
            }
            else if (request == CurrentGameServiceRequest.CreateNewAccount)
            {
                if (_stream_save_game_waiting == null)
                {
                    _stream_save_game_waiting = svstream;
                    if (_context_check_account == null && Context_Url.Length > 0)
                    {
                        _context_check_account = new ODataService<WildWildWestServer.WildWildWestContext>(Context_Url);
                    }
                    if (_context_check_account != null)
                    {
                        StreamBuffer buffer = new StreamBuffer(svstream.PostData);
                        AccountData acc = new AccountData(buffer);
                        _collection_gamedto = new DataServiceCollection<SaveGameDto>(_context_check_account._context);
                        SaveGameDto createdto = SaveGameDto.CreateSaveGameDto((long)acc.AccountID, (int)acc.TotalCoin,
                                                                               (int)acc.HighScore, (short)acc.Location);
                        createdto.UserName = acc.UserName;
                        createdto.AvatarUrl = acc.AvatarUrl;
                        byte[] data = new byte[acc.Data.Length];
                        for (int i = 0; i < acc.Data.Length; i++)
                        {
                            data[i] = acc.Data[i];
                        }
                        createdto.Data = data;
                        _context_check_account._context.AddToSaveGames(createdto);
                        _context_check_account._context.BeginSaveChanges(_context_check_account._context.SaveChangesDefaultOptions, HandleCreateNewUser, _context_check_account._context);
                    }
                    else
                    {
                        svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                }
            }
            else if (request == CurrentGameServiceRequest.SyncSaveToServer)
            {
                if(_stream_save_game_waiting==null)
                {
                    _stream_save_game_waiting = svstream;
                    if(_context_check_account==null && Context_Url.Length >0)
                    {
                        _context_check_account = new ODataService<WildWildWestServer.WildWildWestContext>(Context_Url);
                    }
                    if(_context_check_account!=null)
                    {
                        StreamBuffer buffer = new StreamBuffer(svstream.PostData);
                        AccountData acc = new AccountData(buffer);
                        bool found = false;
                        for (int i = 0; i < _collection_gamedto.Count; i++)
                        {
                            if((ulong)_collection_gamedto[i].ID==acc.AccountID)
                            {
                                found = true;
                                SaveGameDto save = _collection_gamedto[i];
                                save.AvatarUrl = acc.AvatarUrl;
                                save.UserName = acc.UserName;
                                save.HighScore = acc.HighScore;
                                save.ID = (long)acc.AccountID;
                                save.Location = acc.Location;
                                save.Money = acc.TotalCoin;
                                byte[] data = new byte[acc.Data.Length];
                                int offset = 0;
                                for (int k = 0; k < acc.Data.Length; k++)
                                {
                                    data[k] = acc.Data[offset + k];
                                }
                                save.Data = data;
                                _context_check_account.setUserId(acc.AccountID);
                                _context_check_account._context.BeginSaveChanges(_context_check_account._context.SaveChangesDefaultOptions, 
                                    HandleSysSaveToServerComplete,_context_check_account._context);

                            }
                        }
                    }
                }
                else
                {
                    svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
            }
            else if(request==CurrentGameServiceRequest.SyncSaveFromServer)
            {
                if (_stream_save_game_waiting == null)
                {
                    _stream_save_game_waiting = svstream;
                    if (_context_check_account == null && Context_Url.Length > 0)
                    {
                        _context_check_account = new ODataService<WildWildWestServer.WildWildWestContext>(Context_Url);
                    }
                    if (_context_check_account != null)
                    {
                        StreamBuffer buff = new StreamBuffer(svstream.PostData);
                        UInt64 id = buff.readUInt64();

                        _collection_gamedto = new DataServiceCollection<SaveGameDto>(_context_check_account._context);
                        IQueryable<SaveGameDto> query = from t in _context_check_account._context.SaveGames
                                                        where t.ID == (long)id
                                                        select t;
                        _collection_gamedto.LoadCompleted += SynSaveFromServerComplete;
                        _collection_gamedto.LoadAsync(query);
                    }
                }
            }
           




            else 
            {
                svstream.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        #region CHECK_SERVER_URL_FUNCTION
        private async void CheckServerUrl()
        {
            if (_stream_check_server != null)
            {
                /* 0 = use serverReal
                   1 = use serverDev
                   2 = no networl
                 */
                var handler = new HttpClientHandler();
                //config proxy (if supported)
                if (handler.SupportsUseProxy())
                {
                    handler.UseProxy = true;
                }
                var client = new HttpClient(handler);
                client.DefaultRequestHeaders.CacheControl = new CacheControlHeaderValue { NoCache = true };
                HttpResponseMessage resMsg = await client.GetAsync(new Uri(OdataServerDefine.UrlServerRealTest));
                if (resMsg.StatusCode == HttpStatusCode.OK)
                {
                    _stream_check_server.ErrorCode = 0;
                    Context_Url = OdataServerDefine.UrlServerReal;
                }
                else
                {
                    _stream_check_server.ErrorCode = 2;
                }
                Context_Url = OdataServerDefine.UrlServerReal;
                _stream_check_server.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                _stream_check_server = null;
            }
        }
        #endregion
        #region GET_HEADER_COMPLETE
        private void getHeaderComplete(object sender, LoadCompletedEventArgs e)
        {
            if (_stream_check_account != null)
            {
                if (e.Error == null)
                {
                    if (_collection_gamedto != null && _collection_gamedto.Count > 0)
                    {
                        SaveGameDto save = _collection_gamedto[0];
                        AccountData acctmp = new AccountData();
                        acctmp.SetData(save.Data);
                        _stream_check_account.ResponseData = acctmp.getBinary().Data;
                        _stream_check_account.ErrorCode = 0;
                        _stream_check_account.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        _stream_check_account.ErrorCode = 1;
                        _stream_check_account.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                }
                else
                {
                    _stream_check_account.ErrorCode = 1;
                    _stream_check_account.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
                _stream_check_account = null;
            }
        }
        #endregion
        #region HANDLE_CREATER_NEW_USER
        private void HandleCreateNewUser(IAsyncResult ar)
        {
            var svcContext = ar.AsyncState as ODataService<WildWildWestContext>;
            bool error = false;
            try
            {
                if (svcContext != null)
                {
                    svcContext._context.EndSaveChanges(ar);
                }
            }
            catch (DataServiceRequestException ex)
            {
                error = true;
            }
            catch (InvalidOperationException ex)
            {
                error = true;
            }
            catch (Exception ex)
            {
                error = true;
            }
            finally
            {
                if (_stream_save_game_waiting != null)
                {
                    if (error)
                    {
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    else
                    {
                        _stream_save_game_waiting.ErrorCode = 0;
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    _stream_save_game_waiting = null;
                }
            }
        }
        #endregion
        #region HANDLE_SYNC_SAVE_TO_SERVER
        private void HandleSysSaveToServerComplete(IAsyncResult ar)
        {
            bool error = false;
            var svcContext = ar.AsyncState as ODataService<WildWildWestContext>;
            try
            {
                if (svcContext != null)
                {
                    svcContext._context.EndSaveChanges(ar);
                }
            }
            catch (DataServiceRequestException ex)
            {
                error = true;
            }
            catch (InvalidOperationException ex)
            {
                error = true;
            }
            catch (Exception ex)
            {

            }
            finally
            {
                if (_stream_save_game_waiting != null)
                {
                    if (error)
                    {
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                    }
                    else
                    {
                        _stream_save_game_waiting.ErrorCode = 0;
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    _stream_save_game_waiting = null;
                }
            }
        }
        #endregion
        #region HANDLE_SYNC_SAVE_FROM_SERVER
        private void SynSaveFromServerComplete(object sender, LoadCompletedEventArgs e)
        {
            if (_stream_save_game_waiting != null)
            {
                if (e.Error == null)
                {
                    if (_collection_gamedto != null && _collection_gamedto.Count > 0)
                    {
                        SaveGameDto save = _collection_gamedto[0];
                        AccountData acc = new AccountData();

                        acc.SetAvatarURL(save.AvatarUrl);
                        acc.SetUserName(save.UserName);
                        acc.SetTotalCoin(save.Money);
                        acc.SetHighScore(save.HighScore);
                        acc.SetLocation(save.Location);
                        acc.SetAccountID((ulong)save.ID);
                        acc.SetData(save.Data);

                        _stream_save_game_waiting.ResponseData = acc.getBinary().Data;
                        _stream_save_game_waiting.ErrorCode = 0;
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                    else
                    {
                        _stream_save_game_waiting.ErrorCode = 1;
                        _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_RESPONSED;
                    }
                }
                else
                {
                    _stream_save_game_waiting.ErrorCode = 2;
                    _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
                }
                _stream_save_game_waiting = null;
            }
            else
            {
                _stream_save_game_waiting.Status = (int)ServerStreamStatus.STREAM_STATUS_ERROR;
            }
        }
        #endregion


    }

}